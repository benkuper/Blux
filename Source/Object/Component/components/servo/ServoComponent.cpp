/*
  ==============================================================================

    ServoComponent.cpp
    Created: 26 Sep 2020 2:21:12pm
    Author:  bkupe

  ==============================================================================
*/

#include "ServoComponent.h"

ServoComponent::ServoComponent(var params) :
    ObjectComponent(getTypeString(), SERVO,  params)
{
}

ServoComponent::~ServoComponent()
{
}
