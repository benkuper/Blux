/*
  ==============================================================================

    SceneAction.h
    Created: 8 Oct 2020 3:32:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Sequence/layers/action/Action.h"

class SceneAction :
    public Action
{
public:
    SceneAction(var params);
    ~SceneAction();

    enum ActionType { LOAD_SCENE, LOAD_NEXT_SCENE };
    ActionType actionType;

    TargetParameter* scene;
    FloatParameter* time;

    void triggerInternal() override;
    static SceneAction* create(var params) { return new SceneAction(params); }
};