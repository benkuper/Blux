/*
  ==============================================================================

    TimedEffect.h
    Created: 9 Oct 2020 9:16:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include  "../../Effect.h"

class TimedEffect :
    public Effect,
    public Timer
{
public:
    TimedEffect(const String &name, var params = var());
    virtual ~TimedEffect();

    FloatParameter* speed;

    FloatParameter* offsetByID;
    FloatParameter* offsetByValue;


    double timeAtLastUpdate;
    double curTime;

    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values) override;
    virtual var getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, int id, var value, float time) { return value;  }



    virtual void timerCallback() override;  
    virtual void addTime();

    virtual void onContainerParameterChangedInternal(Parameter* p) override;
};