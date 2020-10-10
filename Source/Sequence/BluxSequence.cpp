/*
  ==============================================================================

    BluxSequence.cpp
    Created: 5 Oct 2020 2:42:07pm
    Author:  bkupe

  ==============================================================================
*/

#include "BluxSequence.h"

#include "layers/action/ActionLayer.h"
#include "layers/effect/EffectLayer.h"
#include "layers/automation/AutomationLayer.h"
#include "layers/colorsource/ColorSourceLayer.h"
#include "ui/BluxSequenceEditor.h"
#include "Object/Object.h"

BluxSequence::BluxSequence() :
	manualStartAtLoad(false)
{
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Effect", &EffectLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Automation", &AutomationLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Color Source", &ColorSourceLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Actions", &ActionLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &AudioLayer::create, this, true));

}

BluxSequence::~BluxSequence()
{
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

void BluxSequence::endLoadFile()
{
	if (manualStartAtLoad) return;
	Sequence::endLoadFile();
}

InspectableEditor* BluxSequence::getEditor(bool isRoot)
{
	return new BluxSequenceEditor(this, isRoot);
}
