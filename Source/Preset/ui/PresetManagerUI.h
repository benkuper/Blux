/*
  ==============================================================================

    PresetManagerUI.h
    Created: 30 Sep 2020 2:15:56pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../PresetManager.h"
#include "PresetUI.h"

class PresetManagerUI :
    public BaseManagerShapeShifterUI<PresetManager, Preset, PresetUI>
{
public:
    PresetManagerUI(const String& name);
    ~PresetManagerUI();

    static PresetManagerUI* create(const String& name) { return new PresetManagerUI(name); }
};