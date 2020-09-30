/*
  ==============================================================================

    NoiseEffect.cpp
    Created: 30 Sep 2020 2:57:23pm
    Author:  bkupe

  ==============================================================================
*/

#include "NoiseEffect.h"
#include "Object/Component/ObjectComponent.h"

NoiseEffect::NoiseEffect(var params) :
    Effect(getTypeString(), params)
{
    type = addEnumParameter("Noise Type", "Type of noise to use");
    type->addOption("Perlin", PERLIN)->addOption("Sine", SINE);

    amplitude = addFloatParameter("Amplitude", "Amplitude of the noise", 1, 0);
    frequency = addFloatParameter("Frequency", "Frequency of the noise", 1, .0001f);
}

NoiseEffect::~NoiseEffect()
{
}

void NoiseEffect::processComponentValues(ObjectComponent* c, var& values)
{
    values[0] = (float)values[0] + amplitude->floatValue();
}
