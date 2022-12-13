/*
  ==============================================================================

	RawDataBlockUI.cpp
	Created: 13 Dec 2022 6:24:21pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"


RawDataBlockUI::RawDataBlockUI(RawDataBlock* b) :
	LayerBlockUI(b),
	rawDataBlock(b)
{
	bgColor = BG_COLOR.darker(.4f);

	rawDataBlock->addAsyncRawDataBlockListener(this);
}

RawDataBlockUI::~RawDataBlockUI()
{
	if (!inspectable.wasObjectDeleted()) rawDataBlock->removeAsyncRawDataBlockListener(this);
}


void RawDataBlockUI::paint(Graphics& g)
{
	LayerBlockUI::paint(g);
}

void RawDataBlockUI::paintOverChildren(Graphics& g)
{
	LayerBlockUI::paintOverChildren(g);
}

void RawDataBlockUI::resizedBlockInternal()
{

}

void RawDataBlockUI::newMessage(const RawDataBlock::RawDataBlockEvent& e)
{
	switch (e.type)
	{
	case RawDataBlock::RawDataBlockEvent::LOADED:
		break;
	}
}