/*
  ==============================================================================

	NoiseEffect.cpp
	Created: 30 Sep 2020 2:57:23pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

NoiseEffect::NoiseEffect(var params) :
	TimedEffect(getTypeString(), params)
{
	type = effectParams.addEnumParameter("Noise Type", "Type of noise to use");
	type->addOption("Sine", SINE)->addOption("Perlin", PERLIN);

	frequency = effectParams.addFloatParameter("Frequency", "Frequency of the noise. This will act weirdly when animating it !", 1);
	amplitude = effectParams.addFloatParameter("Amplitude", "Amplitude of the noise", 1, 0);
	valueOffset = effectParams.addFloatParameter("Value Offset", "This defines the center value of the noise. For example, 0 will make the sine oscillate from -amplitude/2 to +amplitude/2", .5f);

	offsetByID->defaultUI = FloatParameter::TIME;
	//offsetByValue->defaultUI = FloatParameter::TIME;

	perlin.reset(new siv::PerlinNoise());
}

NoiseEffect::~NoiseEffect()
{
}

void NoiseEffect::processedComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time, float originalTime)
{
	if (c->mainParameter != nullptr) return;

	NoiseType t = (NoiseType)(int)GetLinkedValue(type);// type->getValueDataAsEnum<NoiseType>();
	float noiseVal = 0;
	switch (t)
	{
	case PERLIN:
		noiseVal = perlin->noise(time) * 2;
		break;

	case SINE:
		noiseVal = sinf(time * float_Pi) * .5f;
		break;
	}
	

	var val = noiseVal * (float)GetLinkedValueO(amplitude);
	targetValues.set(c->mainParameter, val);
}

float NoiseEffect::getCurrentTime(Object* o, ObjectComponent* c, int id, float timeOverride)
{
	return TimedEffect::getCurrentTime(o, c, id, timeOverride) * (float)effectParams.getLinkedValue(frequency, c->object, c->object->globalID->intValue());
}
