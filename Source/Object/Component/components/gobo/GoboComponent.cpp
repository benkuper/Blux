/*
  ==============================================================================

    GoboComponent.cpp
    Created: 26 Sep 2020 2:20:07pm
    Author:  bkupe

  ==============================================================================
*/

#include "GoboComponent.h"

GoboComponent::GoboComponent(var params) :
    ObjectComponent(getTypeString(), params)
{
}

GoboComponent::~GoboComponent()
{
}
