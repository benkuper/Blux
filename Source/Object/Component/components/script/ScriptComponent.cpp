/*
  ==============================================================================

    ScriptComponent.cpp
    Created: 28 Sep 2020 9:07:14pm
    Author:  bkupe

  ==============================================================================
*/

#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(var params) :
    ObjectComponent(getTypeString(), SCRIPT, params)
{
}

ScriptComponent::~ScriptComponent()
{
}
