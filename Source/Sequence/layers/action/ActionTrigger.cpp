/*
  ==============================================================================

    ActionTrigger.cpp
    Created: 12 Oct 2020 10:55:48am
    Author:  bkupe

  ==============================================================================
*/

#include "ActionTrigger.h"
#include "Common/CommonIncludes.h"

ActionTrigger::ActionTrigger() :
    TimeTrigger("Action")
{
    actionManager.reset(new ActionManager());
    saveAndLoadRecursiveData = true;
    addChildControllableContainer(actionManager.get());
}

ActionTrigger::~ActionTrigger()
{
}

void ActionTrigger::triggerInternal()
{
    actionManager->triggerAll();
}
