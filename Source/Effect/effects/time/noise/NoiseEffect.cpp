/*
  ==============================================================================

	NoiseEffect.cpp
	Created: 30 Sep 2020 2:57:23pm
	Author:  bkupe

  ==============================================================================
*/

NoiseEffect::NoiseEffect(var params) :
	TimedEffect(getTypeString(), params)
{
	type = effectParams.addEnumParameter("Noise Type", "Type of noise to use");
	type->addOption("Sine", SINE)->addOption("Perlin", PERLIN);

	frequency = effectParams.addFloatParameter("Frequency", "Frequency of the noise. This will act weirdly when animating it !", 1);
	amplitude = effectParams.addFloatParameter("Amplitude", "Amplitude of the noise", 1, 0);
	valueOffset = effectParams.addFloatParameter("Value Offset", "This defines the center value of the noise. For example, 0 will make the sine oscillate from -amplitude/2 to +amplitude/2", .5f);

	offsetByID->defaultUI = FloatParameter::TIME;
	offsetByValue->defaultUI = FloatParameter::TIME;

	filterManager->componentSelector.allowedComponents.removeAllInstancesOf(ComponentType::COLOR);
	filterManager->componentSelector.selectedComponents.set(ComponentType::INTENSITY, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::PANTILT, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::SERVO, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::STEPPER, true);

	perlin.reset(new siv::PerlinNoise());
}

NoiseEffect::~NoiseEffect()
{
}

var NoiseEffect::getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, var value, int id, float time)
{
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

	return (float)GetLinkedValue(valueOffset) + noiseVal * (float)GetLinkedValue(amplitude);
}

float NoiseEffect::getCurrentTime(Object * o, float timeOverride)
{
	return TimedEffect::getCurrentTime(o, timeOverride) * frequency->floatValue();
}
