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
    Action(var params);
    virtual ~Action();

    String typeString;


    String getTypeString() const override { return typeString; }
};