/*
  ==============================================================================

    GoboComponent.cpp
    Created: 26 Sep 2020 2:20:07pm
    Author:  bkupe

  ==============================================================================
*/

#include "GoboComponent.h"

GoboComponent::GoboComponent(Object* o, var params) :
    ObjectComponent(o, getTypeString(), GOBO, params)
{
}

GoboComponent::~GoboComponent()
{
}
