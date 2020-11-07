/*
  ==============================================================================

    ColorComponent.cpp
    Created: 26 Sep 2020 2:20:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "ColorComponent.h"
#include "Color/ColorSource/ColorSource.h"
#include "Color/PixelShape/PixelShape.h"

ColorComponent::ColorComponent(var params) :
    ObjectComponent(getTypeString(), COLOR, params)
{
    resolution = addIntParameter("Resolution", "Number of different colors/pixels for this object", 1, 1);
}

ColorComponent::~ColorComponent()
{
}
