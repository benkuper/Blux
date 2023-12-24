/*
  ==============================================================================

	SpatManagerView.cpp
	Created: 24 Dec 2023 10:05:15am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

SpatManagerView::SpatManagerView(SpatManager* manager) :
	BaseManagerViewUI(manager->niceName, manager)
{
	transparentBG = true;
	autoFilterHitTestOnItems = true;
	validateHitTestOnNoItem = false;

	useCheckersAsUnits = true;
	updatePositionOnDragMove = true;

	addExistingItems();
}

SpatManagerView::~SpatManagerView()
{
}
