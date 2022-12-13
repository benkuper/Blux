/*
  ==============================================================================

    RawDataLayer.cpp
    Created: 12 Dec 2022 5:37:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataLayer::RawDataLayer(Sequence* s, var params) :
	SequenceLayer(s, "Raw Data"),
	blockManager(this)
{
	saveAndLoadRecursiveData = true;
	addChildControllableContainer(&blockManager);
}

RawDataLayer::~RawDataLayer()
{
}

SequenceLayerTimeline* RawDataLayer::getTimelineUI()
{
	return new RawDataLayerTimeline(this);
}

