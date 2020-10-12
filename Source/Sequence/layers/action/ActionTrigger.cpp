/*
  ==============================================================================

    ActionTrigger.cpp
    Created: 12 Oct 2020 10:55:48am
    Author:  bkupe

  ==============================================================================
*/

#include "ActionTrigger.h"

ActionTrigger::ActionTrigger() :
    TimeTrigger("Action")
{
    saveAndLoadRecursiveData = true;
    addChildControllableContainer(&actionManager);
}

ActionTrigger::~ActionTrigger()
{
}

void ActionTrigger::triggerInternal()
{
    actionManager.triggerAll();
}
