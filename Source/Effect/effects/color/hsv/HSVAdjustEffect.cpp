/*
  ==============================================================================

    HSVAdjustEffect.cpp
    Created: 2 Nov 2020 6:24:15pm
    Author:  bkupe

  ==============================================================================
*/

HSVAdjustEffect::HSVAdjustEffect(var params) :
    ColorEffect(params)
{
    hue = effectParams.addFloatParameter("Hue Shift", "Shifts the hue, 1 is full rainbow rotation", 0);
    saturation = effectParams.addFloatParameter("Saturation", "Shifts the hue, 1 is full rainbow rotation", 0, -1, 1);
    brightness = effectParams.addFloatParameter("Brightness", "Shifts the hue, 1 is full rainbow rotation", 0, -1, 1);
}

HSVAdjustEffect::~HSVAdjustEffect()
{
}

void HSVAdjustEffect::processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
    int numColors = colors.size();
    for (int i=0;i<numColors;i++)
    {
        colors.set(i, colors[i].withRotatedHue(GetLinkedValue(hue))
            .withSaturation(jlimit<float>(0, 1, colors[i].getSaturation() + saturation->floatValue()))
            .withBrightness(jlimit<float>(0, 1, colors[i].getBrightness() + brightness->floatValue()))
        );

    }
}
