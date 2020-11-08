/*
  ==============================================================================

    StepperComponent.h
    Created: 26 Sep 2020 2:21:17pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class StepperComponent :
    public ObjectComponent
{
public:
    StepperComponent(Object* o, var params);
    ~StepperComponent();

    String getTypeString() const override { return "Stepper"; }
    static StepperComponent* create(Object * o, var params) { return new StepperComponent(o, params); }
};