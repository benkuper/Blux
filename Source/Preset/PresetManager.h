/*
  ==============================================================================

    PresetManager.h
    Created: 30 Sep 2020 2:15:37pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Preset.h"

class PresetManager :
    public BaseManager<Preset>
{
public:
    juce_DeclareSingleton(PresetManager, true);
    PresetManager();
    ~PresetManager();

};