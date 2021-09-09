/*
  ==============================================================================

    ColorEffect.cpp
    Created: 9 Nov 2020 12:53:03pm
    Author:  bkupe

  ==============================================================================
*/

#include "ColorEffect.h"

ColorEffect::ColorEffect(const String& name, var params) :
    Effect(name, params),
    fillWithOriginalColors(true)
{
    filterManager.componentSelector.selectedComponents.set(ComponentType::COLOR, true);
}

ColorEffect::~ColorEffect()
{
}

var ColorEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{
    if (!values[0].isArray()) return values;

    Array<Colour, CriticalSection> colors;
    colors.resize(values.size());


    if (fillWithOriginalColors)
    {
        for (int i = 0; i < values.size(); i++) colors.set(i, Colour::fromFloatRGBA(values[i][0], values[i][1], values[i][2], values[i][3]));
    }

    processedEffectColorsInternal(colors, o, (ColorComponent *)c, id, time);

    var result;
    for (auto& col : colors)
    {
        var cv;
        cv.append(col.getFloatRed());
        cv.append(col.getFloatGreen());
        cv.append(col.getFloatBlue());
        cv.append(col.getFloatAlpha());
        result.append(cv);
    }
    return result;
}
