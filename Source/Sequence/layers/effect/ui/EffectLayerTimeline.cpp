/*
  ==============================================================================

    EffectLayerTimeline.cpp
    Created: 10 Oct 2020 10:48:01am
    Author:  bkupe

  ==============================================================================
*/

#include "EffectLayerTimeline.h"

EffectLayerTimeline::EffectLayerTimeline(EffectLayer* l) :
    SequenceLayerTimeline(l),
    effectLayer(l),
    blockManagerUI(this)
{
    addAndMakeVisible(&blockManagerUI);
}

EffectLayerTimeline::~EffectLayerTimeline()
{
}

void EffectLayerTimeline::resized()
{
    blockManagerUI.setBounds(getLocalBounds());
}

void EffectLayerTimeline::updateContent()
{
    blockManagerUI.updateContent();
}

void EffectLayerTimeline::updateMiniModeUI()
{
    blockManagerUI.setMiniMode(item->miniMode->boolValue());
}

void EffectLayerTimeline::addSelectableComponentsAndInspectables(Array<Component*>& selectables, Array<Inspectable*>& inspectables)
{
    blockManagerUI.addSelectableComponentsAndInspectables(selectables, inspectables);
}
