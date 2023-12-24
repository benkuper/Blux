/*
  ==============================================================================

	SpatItemViewUI.cpp
	Created: 24 Dec 2023 10:05:26am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

SpatItemViewUI::SpatItemViewUI(SpatItem* item) :
	BaseItemMinimalUI(item)
{
	autoHideWhenDragging = false;
	drawEmptyDragIcon = true;
	syncWithItemSize = true;

	setSize(item->viewUISize->getPoint().x, item->viewUISize->getPoint().y);
}

SpatItemViewUI::~SpatItemViewUI()
{
}

void SpatItemViewUI::paint(Graphics& g)
{
	Colour c = item->itemColor->getColor();
	g.fillAll(c.withAlpha(.2f));
	g.setColour(c);
	g.drawRect(getLocalBounds(), 1);
}
