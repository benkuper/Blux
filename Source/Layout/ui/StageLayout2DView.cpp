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
    animateItemOnAdd = false;

    canZoom = true;
    contentIsFlexible = true;

    useCheckersAsUnits = true;
    updatePositionOnDragMove = true;
 
    centerUIAroundPosition = true;

    iconSizeUI.reset(StageLayoutManager::getInstance()->iconSize->createSlider());
    iconSizeUI->useCustomBGColor = true;
    iconSizeUI->customBGColor = BG_COLOR.darker(.5f);
    addAndMakeVisible(iconSizeUI.get());

    showFiltersUI.reset(StageLayoutManager::getInstance()->showFilters->createToggle());
    addAndMakeVisible(showFiltersUI.get());

    addChildComponent(&filterStageView);
    filterStageView.setVisible(StageLayoutManager::getInstance()->showFilters->boolValue());

    StageLayoutManager::getInstance()->addAsyncContainerListener(this);

    addExistingItems();
}

StageLayout2DView::~StageLayout2DView()
{
    StageLayoutManager::getInstance()->removeAsyncContainerListener(this);
}

void StageLayout2DView::addItemUIInternal(Object2DView* ui)
{
    if(filterStageView.isVisible()) filterStageView.toFront(false);
}


void StageLayout2DView::resized()
{
    BaseManagerShapeShifterViewUI::resized();

    Rectangle<int> r = getLocalBounds().removeFromTop(headerSize);
    iconSizeUI->setBounds(r.removeFromLeft(200).reduced(3));
    r.removeFromLeft(8);
    showFiltersUI->setBounds(r.removeFromLeft(100).reduced(3));

    if (filterStageView.isVisible())
    {
        filterStageView.setBounds(getLocalBounds());
        filterStageView.setBoundsInView(getViewBounds(getLocalBounds()));
        filterStageView.toFront(false);
    }
}


void StageLayout2DView::newMessage(const ContainerAsyncEvent& e)
{
    switch (e.type)
    {
    case ContainerAsyncEvent::ControllableFeedbackUpdate:
        if (e.targetControllable == StageLayoutManager::getInstance()->showFilters)
        {
            filterStageView.setVisible(StageLayoutManager::getInstance()->showFilters->boolValue());
        }
        else if (e.targetControllable == StageLayoutManager::getInstance()->iconSize)
        {
            for (auto& ui : itemsUI)
            {
                float s = StageLayoutManager::getInstance()->iconSize->floatValue();
                ui->setSize(s, s);
                updateViewUIPosition(ui);
            }
        }
        break;
    }
}
