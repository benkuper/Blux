/*
  ==============================================================================

    ColorComponent.cpp
    Created: 26 Sep 2020 2:20:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "ColorComponent.h"

ColorComponent::ColorComponent(var params) :
    ObjectComponent(getTypeString(), COLOR, params)
{
    color = addColorParameter("Color", "The color that will be converted to RGB channels or whatever the output format is.", Colours::black);
    addComputedParameter(new ColorParameter("Color", "Color", Colours::black), 1, color);
}

ColorComponent::~ColorComponent()
{
}
