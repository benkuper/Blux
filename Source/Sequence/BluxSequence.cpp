/*
  ==============================================================================

	BluxSequence.cpp
	Created: 5 Oct 2020 2:42:07pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"
#include "Audio/AudioManager.h"
#include "Scene/Scene.h"
#include "BluxSequence.h"

BluxSequence::BluxSequence() :
	manualStartAtLoad(false)
{
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Effect", &EffectLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Automation", &AutomationLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Color Source", &ColorSourceLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Actions", &ActionLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", RawDataLayer::getTypeStringStatic(), &RawDataLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &AudioLayer::create, this, true));

	layerManager->addManagerListener(this);

	setAudioDeviceManager(&AudioManager::getInstance()->am);
}

BluxSequence::~BluxSequence()
{
	layerManager->removeManagerListener(this);
}

bool BluxSequence::isAffectingObject(Object* o)
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (!layerManager->items[i]->enabled->boolValue()) continue;
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			if (e->filterManager->isAffectingObject(o)) return true;
		}
	}

	return false;
}

Array<ChainVizTarget*> BluxSequence::getChainVizTargetsForObjectAndComponent(Object* o, ComponentType c)
{
	Array<ChainVizTarget*> result;
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			result.addArray(e->getChainVizTargetsForObjectAndComponent(o, c));
		}
	}
	return result;
}

void BluxSequence::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier)
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (!layerManager->items[i]->enabled->boolValue()) continue;
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			e->processComponent(o, c, values, weightMultiplier);
		}
	}
}

void BluxSequence::processRawData()
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (!layerManager->items[i]->enabled->boolValue()) continue;
		if (RawDataLayer* ri = dynamic_cast<RawDataLayer*>(layerManager->items[i])) ri->processRawData();
	}
}

void BluxSequence::itemAdded(SequenceLayer* layer)
{
	if (AudioLayer* al = dynamic_cast<AudioLayer*>(layer))
	{
		al->setAudioProcessorGraph(&AudioManager::getInstance()->graph, AUDIO_OUTPUT_GRAPH_ID);
	}
}

void BluxSequence::itemsAdded(Array<SequenceLayer*> layers)
{
	for (auto& layer : layers)
	{
		if (AudioLayer* al = dynamic_cast<AudioLayer*>(layer))
		{
			al->setAudioProcessorGraph(&AudioManager::getInstance()->graph, AUDIO_OUTPUT_GRAPH_ID);
		}
	}
}

void BluxSequence::handleStartAtLoad()
{
	//do nothing for start at load, handled by scene manager
}

String BluxSequence::getPanelName() const
{
	if (Scene* s = ControllableUtil::findParentAs<Scene>(parentContainer)) return s->niceName;
	return niceName;
}

ChainVizComponent* BluxSequence::createVizComponent(Object* o, ComponentType ct, ChainVizTarget::ChainVizType type)
{
	return new BluxSequenceChainVizUI(this, o, ct, type);
}

InspectableEditor* BluxSequence::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new BluxSequenceEditor(this, isRoot);
}
