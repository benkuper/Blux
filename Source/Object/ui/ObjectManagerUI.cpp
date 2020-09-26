/*
  ==============================================================================

    ObjectManagerUI.cpp
    Created: 26 Sep 2020 7:51:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectManagerUI.h"

ObjectManagerUI::ObjectManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, ObjectManager::getInstance())
{
}

ObjectManagerUI::~ObjectManagerUI()
{
}
