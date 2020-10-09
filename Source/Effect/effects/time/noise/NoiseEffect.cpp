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
	TimedEffect(getTypeString(), params)
{
	type = addEnumParameter("Noise Type", "Type of noise to use");
	type->addOption("Sine", SINE)->addOption("Perlin", PERLIN);

	amplitude = addFloatParameter("Amplitude", "Amplitude of the noise", 1, 0);
	offsetByID->defaultUI = FloatParameter::TIME;
	offsetByValue->defaultUI = FloatParameter::TIME;

	filterManager.componentSelector.selectedComponents.set(ObjectComponent::INTENSITY, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::PANTILT, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::SERVO, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::STEPPER, true);
}

NoiseEffect::~NoiseEffect()
{
}

var NoiseEffect::getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, int id, var value, float time)
{
	NoiseType t = type->getValueDataAsEnum<NoiseType>();
	float noiseVal = 0;
	switch (t)
	{
	case PERLIN:
		noiseVal = perlin.noise(time) * 2;
		break;

	case SINE:
		noiseVal = sinf(time * float_Pi);
		break;
	}

	return (float)value + noiseVal * amplitude->floatValue();
}