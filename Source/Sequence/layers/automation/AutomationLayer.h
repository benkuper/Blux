/*
  ==============================================================================

    AutomationLayer.h
    Created: 5 Oct 2020 4:03:08pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class AutomationLayer :
    public SequenceLayer
{
public:
    AutomationLayer(Sequence* s, var params);
    ~AutomationLayer();

    LayerBlockManager blocks;

    String getTypeString() const override { return "Automation"; }

    static AutomationLayer* create(Sequence* s, var params) { return new AutomationLayer(s, params); }
};