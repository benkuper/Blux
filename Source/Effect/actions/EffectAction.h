/*
  ==============================================================================

    EffectAction.h
    Created: 8 Oct 2020 3:31:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class EffectAction :
    public Action
{
public:
    EffectAction(var params);
    ~EffectAction();

    enum ActionType { ENABLE_EFFECT, DISABLE_EFFECT, SET_EFFECT_WEIGHT };
    ActionType actionType;

    TargetParameter* effect;
    FloatParameter* weight;

    void triggerInternal() override;
    void setValueInternal(var value) override;

    static void showMenuAndGetEffect(ControllableContainer*, std::function<void(ControllableContainer *)> returnFunc);

    static EffectAction* create(var params) { return new EffectAction(params); }
};