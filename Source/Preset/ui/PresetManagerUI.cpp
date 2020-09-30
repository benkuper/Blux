/*
  ==============================================================================

    PresetManagerUI.cpp
    Created: 30 Sep 2020 2:15:56pm
    Author:  bkupe

  ==============================================================================
*/

#include "PresetManagerUI.h"

PresetManagerUI::PresetManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, PresetManager::getInstance())
{
}

PresetManagerUI::~PresetManagerUI()
{
}
