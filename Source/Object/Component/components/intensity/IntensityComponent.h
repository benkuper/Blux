/*
  ==============================================================================

    IntensityComponent.h
    Created: 26 Sep 2020 2:20:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class IntensityComponent :
    public ObjectComponent
{
public:
    IntensityComponent(var params);
    ~IntensityComponent();

    FloatParameter* value;

    String getTypeString() const override { return "Intensity"; }
    static IntensityComponent* create(var params) { return new IntensityComponent(params); }
};