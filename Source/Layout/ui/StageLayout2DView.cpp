/*
  ==============================================================================

    StageLayout2DView.cpp
    Created: 6 Oct 2020 9:49:35pm
    Author:  bkupe

  ==============================================================================
*/

#include "StageLayout2DView.h"
#include "../StageLayoutManager.h"

StageLayout2DView::StageLayout2DView(const String& name) :
    BaseManagerShapeShifterViewUI(name, ObjectManager::getInstance())
{
    canZoom = true;
    contentIsFlexible = true;

    useCheckersAsUnits = true;
    updatePositionOnDragMove = true;

    iconSizeUI.reset(StageLayoutManager::getInstance()->iconSize->createSlider());
    addAndMakeVisible(iconSizeUI.get());

    addExistingItems();
}

StageLayout2DView::~StageLayout2DView()
{
}

void StageLayout2DView::resizedInternalHeader(Rectangle<int>& r)
{
    iconSizeUI->setBounds(r.removeFromLeft(200).reduced(3));
}
