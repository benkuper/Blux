/*
  ==============================================================================

    ActionLayer.cpp
    Created: 5 Oct 2020 4:02:52pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionLayer.h"
#include "../../BluxSequence.h"

ActionLayer::ActionLayer(Sequence* s, var params) :
    TriggerLayer(s, "Action", params)
{
    setManager(new ActionTriggerManager(this, s));
}

ActionLayer::~ActionLayer()
{
}

ActionTriggerManager::ActionTriggerManager(ActionLayer* layer, Sequence* sequence) :
    TimeTriggerManager(layer, sequence)
{
}

TimeTrigger* ActionTriggerManager::createItem()
{
    return new ActionTrigger();
}
