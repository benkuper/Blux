/*
  ==============================================================================

    StepperComponent.cpp
    Created: 26 Sep 2020 2:21:17pm
    Author:  bkupe

  ==============================================================================
*/

#include "StepperComponent.h"

StepperComponent::StepperComponent(Object* o, var params) :
    ObjectComponent(o, getTypeString(), STEPPER, params)
{
}

StepperComponent::~StepperComponent()
{
}
