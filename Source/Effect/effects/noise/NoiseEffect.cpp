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
	offsetByID = addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);
	offsetByValue = addFloatParameter("Time Offset By Value", "Time Offset by parameter inside a component", 0);
	offsetByID->defaultUI = FloatParameter::TIME;
	offsetByValue->defaultUI = FloatParameter::TIME;

	timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
	startTimer(20);

	filterManager.componentSelector.selectedComponents.set(ObjectComponent::INTENSITY, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::PANTILT, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::SERVO, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::STEPPER, true);
}

NoiseEffect::~NoiseEffect()
{
}

var NoiseEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values)
{
	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].isArray()) continue;
		
		float noiseVal = 0;
		float time = curTime + offsetByID->floatValue() * id + offsetByValue->floatValue() * i;
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

		values[i] = (float)values[i] + noiseVal * amplitude->floatValue();
	}

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

void NoiseEffect::timerCallback()
{
	double newTime = Time::getMillisecondCounter() / 1000.0;
	curTime += (newTime - timeAtLastUpdate) * frequency->floatValue();
	timeAtLastUpdate = newTime;
}
