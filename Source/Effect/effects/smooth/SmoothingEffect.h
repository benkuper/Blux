/*
  ==============================================================================

    SmoothingEffect.h
    Created: 17 Jan 2022 11:53:19am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SmoothingEffect :
    public Effect
{
public:
    SmoothingEffect(var params = var());
    virtual ~SmoothingEffect();

    HashMap<ObjectComponent*, double> prevTimes;

    FloatParameter* smoothing;
    FloatParameter* fallSmoothing;

    Trigger* reset;

    virtual void onContainerTriggerTriggered(Trigger* t) override;
    virtual void onContainerParameterChangedInternal(Parameter* p) override;

    void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Smoothing"; }
    static SmoothingEffect* create(var params) { return new SmoothingEffect(params); }
};