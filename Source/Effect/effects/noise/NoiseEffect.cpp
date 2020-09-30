/*
  ==============================================================================

    NoiseEffect.cpp
    Created: 30 Sep 2020 2:57:23pm
    Author:  bkupe

  ==============================================================================
*/

#include "NoiseEffect.h"
#include "Object/Object.h"

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

void NoiseEffect::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    float noiseVal = 0;
    float time = (Time::getMillisecondCounter() / 1000.0f) * frequency->floatValue();
    NoiseType t = type->getValueDataAsEnum<NoiseType>();
    switch (t)
    {
    case PERLIN:
        noiseVal = perlin.noise(time) * 2;
        break;

    case SINE:
        noiseVal = sinf(time * float_Pi);
        break;
    }

    values[0] = (float)values[0] + noiseVal * amplitude->floatValue();
}