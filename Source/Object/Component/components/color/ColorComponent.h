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
    ColorComponent(Object * o, var params);
    ~ColorComponent();

    IntParameter* resolution;

    Array<Colour, CriticalSection> colors;
    std::unique_ptr<ColorSource> prevColorSource; //for transitionning

    std::unique_ptr<ColorSource> colorSource;
    std::unique_ptr<PixelShape> pixelShape;

    void setupSource(const String &type, ColorSource * templateRef = nullptr);
    void setupShape();
    void setupFromJSONDefinition(var definition) override;

    void update() override;
    virtual var getOriginalComputedValues() override;

    void onContainerParameterChangedInternal(Parameter* p) override;


    String getTypeString() const override { return "Color"; }
    static ColorComponent* create(Object * o, var params) { return new ColorComponent(o, params); }

    InspectableEditor* getEditor(bool isRoot) override;
};