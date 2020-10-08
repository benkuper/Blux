/*
  ==============================================================================

    Action.h
    Created: 8 Oct 2020 3:33:09pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Action :
    public TimeTrigger
{
public:
    Action(const String &name = "Action", var params = var());
    virtual ~Action();
};