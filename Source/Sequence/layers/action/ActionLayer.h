/*
  ==============================================================================

    ActionLayer.h
    Created: 5 Oct 2020 4:02:52pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ActionLayer :
    public TriggerLayer
{
public:
    ActionLayer(Sequence* s, var params);
    ~ActionLayer();

    String getTypeString() const override { return "Action"; }
    static ActionLayer* create(Sequence* s, var params) { return new ActionLayer(s, params); }
};