/*
  ==============================================================================

    SequenceAction.h
    Created: 8 Oct 2020 3:32:39pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SequenceAction :
    public Action
{
public:
    SequenceAction(var params);
    ~SequenceAction();

    enum ActionType { PLAY_SEQUENCE, STOP_SEQUENCE };
    ActionType actionType;
    TargetParameter* sequence;

    void triggerInternal() override;

    static SequenceAction* create(var params) { return new SequenceAction(params); }
};