/*
  ==============================================================================

    ObjectComponent.h
    Created: 26 Sep 2020 10:03:23am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ObjectComponent :
    public BaseItem
{
public:
    ObjectComponent(String name = "Component", var params = var());
    virtual ~ObjectComponent();

    virtual void setupFromJSONDefinition(var data);
};