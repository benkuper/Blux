/*
  ==============================================================================

    StageLayoutManagerUI.cpp
    Created: 6 Oct 2020 9:50:00pm
    Author:  bkupe

  ==============================================================================
*/

StageLayoutManagerUI::StageLayoutManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, StageLayoutManager::getInstance(), true)
{
    addExistingItems();
}

StageLayoutManagerUI::~StageLayoutManagerUI()
{
}
