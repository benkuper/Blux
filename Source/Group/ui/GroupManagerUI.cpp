/*
  ==============================================================================

    GroupManagerUI.cpp
    Created: 29 Sep 2020 9:46:32pm
    Author:  bkupe

  ==============================================================================
*/

#include "GroupManagerUI.h"

GroupManagerUI::GroupManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, GroupManager::getInstance())
{
    setShowSearchBar(true);
    addExistingItems();
}

GroupManagerUI::~GroupManagerUI()
{
}
