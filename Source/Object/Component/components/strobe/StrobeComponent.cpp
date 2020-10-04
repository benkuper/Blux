/*
  ==============================================================================

    StrobeComponent.cpp
    Created: 26 Sep 2020 2:21:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "StrobeComponent.h"

StrobeComponent::StrobeComponent(var params) :
    ObjectComponent(getTypeString(), STROBE, params)
{
}

StrobeComponent::~StrobeComponent()
{
}
