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
	valueRange = effectParams.addPoint2DParameter("Value Range", "Min and Max for this noise");
	valueRange->setBounds(-1, -1, 1, 1);
	valueRange->canShowExtendedEditor = false;
	var valDefault;
	valDefault.append(0);
	valDefault.append(1);
	valueRange->setDefaultValue(valDefault);

	offsetByID->defaultUI = FloatParameter::TIME;
	//offsetByValue->defaultUI = FloatParameter::TIME;

	perlin.reset(new siv::PerlinNoise());
}

NoiseEffect::~NoiseEffect()
{
}

void NoiseEffect::processComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time, float originalTime)
{
	if (c->mainParameter == nullptr) return;

	NoiseType t = (NoiseType)(int)GetLinkedValue(type);// type->getValueDataAsEnum<NoiseType>();
	float noiseVal = 0;

	//all noise type should generate a value between 0 and 1
	switch (t)
	{
	case PERLIN:
		noiseVal = perlin->noise(time) * 2 + .5f;
		break;

	case SINE:
		noiseVal = sinf(time * float_Pi) * .5f + .5f;
		break;
	}


	var rangeVal = GetLinkedValueO(valueRange);
	float targetVal = jmap<float>(noiseVal, rangeVal[0], rangeVal[1]);
	if (c->mainParameter->isComplex())
	{
		var v;
		for (int i = 0; i < c->mainParameter->value.size(); i++) v.append(targetVal);
		targetValues.set(c->mainParameter, v);
	}
	else
	{
		targetValues.set(c->mainParameter, targetVal);
	}
}

float NoiseEffect::getCurrentTime(Object* o, ObjectComponent* c, int id, float timeOverride)
{
	return TimedEffect::getCurrentTime(o, c, id, timeOverride) * (float)effectParams.getLinkedValue(frequency, c->object, c->object->globalID->intValue());
}
