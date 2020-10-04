/*
  ==============================================================================

    StepperComponent.cpp
    Created: 26 Sep 2020 2:21:17pm
    Author:  bkupe

  ==============================================================================
*/

#include "StepperComponent.h"

StepperComponent::StepperComponent(var params) :
    ObjectComponent(getTypeString(), STEPPER, params)
{
}

StepperComponent::~StepperComponent()
{
}
