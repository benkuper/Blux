/*
  ==============================================================================

    ColorSourceOverrideEffect.h
    Created: 9 Nov 2020 12:54:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ColorEffect.h"

class ColorSource;

class ColorSourceOverrideEffect :
    public ColorEffect
{
public:
    ColorSourceOverrideEffect(var params);
    ~ColorSourceOverrideEffect();

    std::unique_ptr<ColorSource> colorSource;
    ColorSource* templateRef;

    void setupSource(const String& type, ColorSource * templateRef = nullptr);

    void processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1);


    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Override (Color)"; }
    static ColorSourceOverrideEffect* create(var params) { return new ColorSourceOverrideEffect(params); }
};