/*
  ==============================================================================

    IntensityComponent.cpp
    Created: 26 Sep 2020 2:20:14pm
    Author:  bkupe

  ==============================================================================
*/

#include "IntensityComponent.h"

IntensityComponent::IntensityComponent(var params) :
    ObjectComponent(getTypeString(), INTENSITY, params)
{
    value = addFloatParameter("Value", "Value of the intensity. This will automatically be converted to 0-255 or whatever the output protocol is.", .5f, 0, 1);
    addComputedParameter(new FloatParameter("Value", "Computed intensity after all effects applied", .5f, 0, 1), 0, value);
}

IntensityComponent::~IntensityComponent()
{
}
