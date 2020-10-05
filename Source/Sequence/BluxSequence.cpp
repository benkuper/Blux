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

BluxSequence::BluxSequence()
{
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Effect", &EffectLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Automation", &AutomationLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Color Source", &ColorSourceLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Actions", &ActionLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &AudioLayer::create, this, true));

}

BluxSequence::~BluxSequence()
{
}
