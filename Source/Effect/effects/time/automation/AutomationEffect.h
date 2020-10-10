/*
  ==============================================================================

    AutomationEffect.h
    Created: 4 Oct 2020 11:37:20pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../TimedEffect.h"

class AutomationEffect :
    public TimedEffect
{
public:
    AutomationEffect(var params = var());
    ~AutomationEffect();

    enum AutomationType { SINE, PERLIN };

    FloatParameter* length;
    Point2DParameter* range;
    BoolParameter* clipTime;

    Automation automation;

    var getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, var value, int id, float time) override;

    void onContainerParameterChangedInternal(Parameter* p)override;

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Automation"; }

    static AutomationEffect* create(var params) { return new AutomationEffect(params); }

};