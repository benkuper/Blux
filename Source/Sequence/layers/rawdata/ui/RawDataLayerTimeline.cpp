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
	needle.toFront(false);
}

RawDataLayerTimeline::~RawDataLayerTimeline()
{
}

void RawDataLayerTimeline::paintOverChildren(Graphics& g)
{
	SequenceLayerTimeline::paintOverChildren(g);
	if (rawDataLayer->isRecording->boolValue())
	{
		int sx = getXForTime(rawDataLayer->timeAtRecord);
		int tx = getXForTime(item->sequence->currentTime->floatValue());	

		g.setColour(RED_COLOR.withAlpha(.5f));
		g.fillRect(Rectangle<float>(sx, 0, tx - sx, getHeight()));
	}
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

void RawDataLayerTimeline::controllableFeedbackUpdateInternal(Controllable* c)
{
	SequenceLayerTimeline::controllableFeedbackUpdateInternal(c);
	if (c == rawDataLayer->arm)
	{
		needle.timeBarColor = rawDataLayer->arm->boolValue() ? RED_COLOR : needle.defaultTimeBarColor;
		repaint();
	}
	else if (c == item->sequence->currentTime)
	{
		repaint();
	}
}

void RawDataLayerTimeline::addSelectableComponentsAndInspectables(Array<Component*>& selectables, Array<Inspectable*>& inspectables)
{
	blockManagerUI.addSelectableComponentsAndInspectables(selectables, inspectables);
}
