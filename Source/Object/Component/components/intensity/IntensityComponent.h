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
    IntensityComponent(Object* o, var params);
    ~IntensityComponent();

    Array<FloatParameter*> values;

    void setupFromJSONDefinition(var data) override;

    String getTypeString() const override { return "Intensity"; }
    static IntensityComponent* create(Object * o, var params) { return new IntensityComponent(o, params); }
};