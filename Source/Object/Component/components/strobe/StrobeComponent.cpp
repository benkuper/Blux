/*
  ==============================================================================

    StrobeComponent.cpp
    Created: 26 Sep 2020 2:21:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "StrobeComponent.h"

StrobeComponent::StrobeComponent(Object* o, var params) :
    ObjectComponent(o, getTypeString(), STROBE, params)
{
}

StrobeComponent::~StrobeComponent()
{
}
