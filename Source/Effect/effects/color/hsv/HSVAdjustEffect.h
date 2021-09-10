/*
  ==============================================================================

    HSVAdjustEffect.h
    Created: 2 Nov 2020 6:24:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSource;

class HSVAdjustEffect :
    public ColorEffect
{
public:
    HSVAdjustEffect(var params);
    ~HSVAdjustEffect();

    FloatParameter* hue;
    FloatParameter* saturation;
    FloatParameter* brightness;

    void processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1);

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "HSV Adjust"; }
    static HSVAdjustEffect* create(var params) { return new HSVAdjustEffect(params); }
};