/*
  ==============================================================================

    ColorComponent.h
    Created: 26 Sep 2020 2:20:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class ColorComponent :
    public ObjectComponent
{
public:
    ColorComponent(var params);
    ~ColorComponent();

    ColorParameter* color;

    String getTypeString() const override { return "Color"; }
    static ColorComponent* create(var params) { return new ColorComponent(params); }
};