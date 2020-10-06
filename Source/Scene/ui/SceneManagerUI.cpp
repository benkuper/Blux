/*
  ==============================================================================

    SceneManagerUI.cpp
    Created: 26 Sep 2020 7:49:30pm
    Author:  bkupe

  ==============================================================================
*/

#include "SceneManagerUI.h"

SceneManagerUI::SceneManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, SceneManager::getInstance())
{
    addExistingItems();
}

SceneManagerUI::~SceneManagerUI()
{
}
