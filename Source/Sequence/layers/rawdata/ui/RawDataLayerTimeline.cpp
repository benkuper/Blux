/*
  ==============================================================================

	RawDataLayerTimeline.cpp
	Created: 13 Dec 2022 6:23:39pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataLayerTimeline::RawDataLayerTimeline(RawDataLayer* l) :
	SequenceLayerTimeline(l),
	rawDataLayer(l),
	blockManagerUI(this)
{
	addAndMakeVisible(&blockManagerUI);
}

RawDataLayerTimeline::~RawDataLayerTimeline()
{
}

void RawDataLayerTimeline::resized()
{
	blockManagerUI.setBounds(getLocalBounds());
}

void RawDataLayerTimeline::updateContent()
{
	blockManagerUI.updateContent();
}

void RawDataLayerTimeline::updateMiniModeUI()
{
	blockManagerUI.setMiniMode(item->miniMode->boolValue());
}

void RawDataLayerTimeline::addSelectableComponentsAndInspectables(Array<Component*>& selectables, Array<Inspectable*>& inspectables)
{
	blockManagerUI.addSelectableComponentsAndInspectables(selectables, inspectables);
}
