/*
  ==============================================================================

    ObjectManagerUI.cpp
    Created: 26 Sep 2020 7:51:25pm
    Author:  bkupe

  ==============================================================================
*/

ObjectManagerUI::ObjectManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, ObjectManager::getInstance())
{
    setShowSearchBar(true);
    addExistingItems();
}

ObjectManagerUI::~ObjectManagerUI()
{
}
