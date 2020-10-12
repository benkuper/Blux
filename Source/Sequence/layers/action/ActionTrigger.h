/*
  ==============================================================================

    ActionTrigger.h
    Created: 12 Oct 2020 10:55:48am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "Common/Action/ActionManager.h"

class ActionTrigger :
    public TimeTrigger
{
public:
    ActionTrigger();
    ~ActionTrigger();

    ActionManager actionManager;

    void triggerInternal();
};