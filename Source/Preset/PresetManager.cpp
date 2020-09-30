/*
  ==============================================================================

    PresetManager.cpp
    Created: 30 Sep 2020 2:15:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "PresetManager.h"
juce_ImplementSingleton(PresetManager)

PresetManager::PresetManager() :
    BaseManager("Presets")
{
}

PresetManager::~PresetManager()
{
}
