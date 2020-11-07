/*
  ==============================================================================

    ColorComponent.h
    Created: 26 Sep 2020 2:20:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class ColorSource;
class PixelShape;

class ColorComponent :
    public ObjectComponent
{
public:
    ColorComponent(var params);
    ~ColorComponent();

    IntParameter* resolution;
    std::unique_ptr<ColorSource> colorSource;
    std::unique_ptr<PixelShape> pixelShape;

    String getTypeString() const override { return "Color"; }
    static ColorComponent* create(var params) { return new ColorComponent(params); }
};