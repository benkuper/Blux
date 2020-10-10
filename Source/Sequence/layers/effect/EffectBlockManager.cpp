/*
  ==============================================================================

    EffectBlockManager.cpp
    Created: 10 Oct 2020 10:47:23am
    Author:  bkupe

  ==============================================================================
*/

#include "EffectBlockManager.h"
#include "EffectBlock.h"
#include "EffectLayer.h"
#include "Effect/EffectManager.h"

juce_ImplementSingleton(EffectBlockFactory)

EffectBlockManager::EffectBlockManager(EffectLayer * layer) :
    LayerBlockManager(layer, "Blocks")
{
    managerFactory = EffectBlockFactory::getInstance();
}

EffectBlockManager::~EffectBlockManager()
{
}

LayerBlock * EffectBlockManager::createItem()
{
    return new EffectBlock();
}

void EffectBlockManager::addItemInternal(LayerBlock* block, var data)
{
	LayerBlockManager::addItemInternal(block, data);
	EffectBlock * clip = dynamic_cast<EffectBlock *>(block);
	clip->addEffectBlockListener(this);
}

void EffectBlockManager::removeItemInternal(LayerBlock* block)
{
	LayerBlockManager::removeItemInternal(block);
	EffectBlock * clip = dynamic_cast<EffectBlock *>(block);
	clip->removeEffectBlockListener(this);
}

void EffectBlockManager::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	EffectBlock * b = c->getParentAs<EffectBlock >();
	if (b != nullptr)
	{
		if (c == b->time || c == b->coreLength || c == b->loopLength)
		{
			if (!blocksCanOverlap) return;
			computeFadesForBlock(b, true);
		}
	}
}


void EffectBlockManager::effectBlockFadesChanged(EffectBlock* block)
{
    computeFadesForBlock(block, false);
}

void EffectBlockManager::computeFadesForBlock(EffectBlock* block, bool propagate)
{
	int bIndex = items.indexOf(block);

	EffectBlock * prevBlock = bIndex > 0 ? (EffectBlock *)items[bIndex - 1] : nullptr;
	EffectBlock * nextBlock = bIndex < items.size() - 1 ? (EffectBlock *)items[bIndex + 1] : nullptr;

	if (prevBlock != nullptr && prevBlock->time->floatValue() > block->time->floatValue())
	{
		reorderItems();
		computeFadesForBlock(block, propagate);
		return;
	}

	if (nextBlock != nullptr && nextBlock->time->floatValue() < block->time->floatValue())
	{
		reorderItems();
		computeFadesForBlock(block, propagate);
		return;
	}

	if (!block->fadeIn->enabled)
	{
		float fadeIn = prevBlock == nullptr ? 0 : jmax(prevBlock->getEndTime() - block->time->floatValue(), 0.f);
		block->fadeIn->setValue(fadeIn);
	}

	if (!block->fadeOut->enabled)
	{
		float fadeOut = nextBlock == nullptr ? 0 : jmax(block->getEndTime() - nextBlock->time->floatValue(), 0.f);
		block->fadeOut->setValue(fadeOut);
	}

	if (propagate)
	{
		if (prevBlock != nullptr) computeFadesForBlock(prevBlock, false);
		if (nextBlock != nullptr) computeFadesForBlock(nextBlock, false);
	}
}

EffectBlockFactory::EffectBlockFactory()
{
    for (auto& d : EffectFactory::getInstance()->defs)
    {
        defs.add(Factory<LayerBlock>::Definition::createDef(d->menuPath, d->type, &EffectBlock::create)->addParam("effectType", d->type)->addIcon(d->icon));
    }
}
