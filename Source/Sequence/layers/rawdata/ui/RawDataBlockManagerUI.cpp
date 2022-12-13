/*
  ==============================================================================

	RawDataBlockManagerUI.cpp
	Created: 13 Dec 2022 6:24:30pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataBlockManagerUI::RawDataBlockManagerUI(RawDataLayerTimeline* timeline) :
	LayerBlockManagerUI(timeline, &timeline->rawDataLayer->blockManager),
	effectTimeline(timeline)
{
	addExistingItems();
}

RawDataBlockManagerUI::~RawDataBlockManagerUI()
{
}


LayerBlockUI* RawDataBlockManagerUI::createUIForItem(LayerBlock* b)
{
	return new RawDataBlockUI((RawDataBlock*)b);
}

void RawDataBlockManagerUI::mouseDoubleClick(const MouseEvent& e)
{
	//nothing, do not create on double click
}
