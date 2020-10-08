/*
  ==============================================================================

    SequenceAction.cpp
    Created: 8 Oct 2020 3:32:39pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceAction.h"
#include "Sequence/GlobalSequenceManager.h"

SequenceAction::SequenceAction(var params) :
    Action(params),
    sequence(nullptr)
{
    actionType = (ActionType)(int)params.getProperty("actionType", PLAY_SEQUENCE);

    sequence = addTargetParameter("Sequence", "The target sequence to control", GlobalSequenceManager::getInstance());
    sequence->maxDefaultSearchLevel = 0;
    sequence->targetType = TargetParameter::CONTAINER;
}

SequenceAction::~SequenceAction()
{
}

void SequenceAction::triggerInternal()
{
    Sequence* s = sequence != nullptr ? (Sequence*)sequence->targetContainer.get() : nullptr;

    if (s == nullptr) return;

    switch (actionType)
    {
    case PLAY_SEQUENCE:
        s->playTrigger->trigger();
        break;

    case STOP_SEQUENCE:
        s->stopTrigger->trigger();
        break;
    }
}
