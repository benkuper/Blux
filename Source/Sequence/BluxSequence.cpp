/*
  ==============================================================================

	BluxSequence.cpp
	Created: 5 Oct 2020 2:42:07pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"
#include "Audio/AudioManager.h"

BluxSequence::BluxSequence() :
	manualStartAtLoad(false)
{
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Effect", &EffectLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Automation", &AutomationLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Color Source", &ColorSourceLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Actions", &ActionLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", RawDataLayer::getTypeStringStatic(), &RawDataLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &AudioLayer::create, this, true));

	layerManager->addBaseManagerListener(this);

	setAudioDeviceManager(&AudioManager::getInstance()->am);
}

BluxSequence::~BluxSequence()
{
	layerManager->removeBaseManagerListener(this);
}

bool BluxSequence::isAffectingObject(Object* o)
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			if (e->filterManager->isAffectingObject(o)) return true;
		}
	}

	return false;
}

Array<ChainVizTarget*> BluxSequence::getChainVizTargetsForObject(Object* o)
{
	Array<ChainVizTarget*> result;
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			result.addArray(e->getChainVizTargetsForObject(o));
		}
	}
	return result;
}

void BluxSequence::processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier)
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			e->processComponentValues(o, c, values, weightMultiplier);
		}
	}
}

void BluxSequence::itemAdded(SequenceLayer* layer)
{
	AudioLayer* al = dynamic_cast<AudioLayer*>(layer);
	if (al != nullptr)
	{
		al->setAudioProcessorGraph(&AudioManager::getInstance()->graph, AUDIO_OUTPUT_GRAPH_ID);
	}
}

void BluxSequence::endLoadFile()
{
	if (manualStartAtLoad) return;
	Sequence::endLoadFile();
}

ChainVizComponent* BluxSequence::createVizComponent(Object* o, ChainVizTarget::ChainVizType type)
{
	return new BluxSequenceChainVizUI(this, o, type);
}

InspectableEditor* BluxSequence::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new BluxSequenceEditor(this, isRoot);
}
