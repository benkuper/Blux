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
    Effect(getTypeString(), params),
    curTime(0)
{
    type = addEnumParameter("Noise Type", "Type of noise to use");
    type->addOption("Perlin", PERLIN)->addOption("Sine", SINE);

    amplitude = addFloatParameter("Amplitude", "Amplitude of the noise", 1, 0);
    frequency = addFloatParameter("Frequency", "Frequency of the noise", 1, .0001f);
    offset = addFloatParameter("Time Offset", "Time Offset", 0);
    offsetByID = addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);

    timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
    startTimer(20);
}

NoiseEffect::~NoiseEffect()
{
}

var NoiseEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values)
{
    float noiseVal = 0;
    int id = filterManager.getFilteredIDForObject(o);
    float time = curTime + offset->floatValue() + offsetByID->floatValue() * id;
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

    return values;
}

void NoiseEffect::onContainerParameterChangedInternal(Parameter* p)
{
    Effect::onContainerParameterChangedInternal(p);
    if (p == enabled)
    {
        if (enabled->boolValue())
        {
            timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
            curTime = 0;
            startTimer(20);
        }
        else
        {
            stopTimer();
        }
    }
}

void NoiseEffect::hiResTimerCallback()
{
    double newTime = Time::getMillisecondCounterHiRes() / 1000.0;
    curTime += (newTime - timeAtLastUpdate) * frequency->floatValue();
    timeAtLastUpdate = newTime;
}
