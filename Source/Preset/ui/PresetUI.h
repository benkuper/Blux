/*
  ==============================================================================

    PresetUI.h
    Created: 30 Sep 2020 2:16:00pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Preset.h"

class PresetUI :
    public BaseItemUI<Preset>
{
public:
    PresetUI(Preset* item);
    ~PresetUI();
};