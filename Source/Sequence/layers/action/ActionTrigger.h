/*
  ==============================================================================

    ActionTrigger.h
    Created: 12 Oct 2020 10:55:48am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ActionManager;

class ActionTrigger :
    public TimeTrigger
{
public:
    ActionTrigger();
    ~ActionTrigger();

    std::unique_ptr<ActionManager> actionManager;

    void triggerInternal();
};