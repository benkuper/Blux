/*
  ==============================================================================

    Preset.h
    Created: 30 Sep 2020 2:15:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Preset :
    public BaseItem
{
public:
    Preset();
    ~Preset();

    String getTypeString() const override { return "Preset"; }
};