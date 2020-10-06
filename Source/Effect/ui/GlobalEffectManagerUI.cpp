/*
  ==============================================================================

    GlobalEffectManagerUI.cpp
    Created: 30 Sep 2020 1:37:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "GlobalEffectManagerUI.h"

GlobalEffectManagerUI::GlobalEffectManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, GlobalEffectManager::getInstance())
{
    addExistingItems();
}

GlobalEffectManagerUI::~GlobalEffectManagerUI()
{
}
