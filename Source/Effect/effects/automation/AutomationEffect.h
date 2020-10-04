/*
  ==============================================================================

    AutomationEffect.h
    Created: 4 Oct 2020 11:37:20pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Effect.h"

class AutomationEffect :
    public Effect,
    public Timer
{
public:
    AutomationEffect(var params = var());
    ~AutomationEffect();

    enum AutomationType { SINE, PERLIN };

    FloatParameter* length;
    FloatParameter* speed;
    Point2DParameter* range;
    FloatParameter* offsetByID;
    FloatParameter* offsetByValue;
    
    Automation automation;

    double timeAtLastUpdate;
    double curTime;

    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values) override;

    void onContainerParameterChangedInternal(Parameter* p)override;

    virtual void timerCallback() override;

    String getTypeString() const override { return "Automation"; }
    static AutomationEffect* create(var params) { return new AutomationEffect(params); }

};