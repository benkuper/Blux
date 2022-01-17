/*
  ==============================================================================

	SmoothingEffect.cpp
	Created: 11 Sep 2021 5:16:02pm
	Author:  bkupe

  ==============================================================================
*/

SmoothingEffect::SmoothingEffect(var params) :
	Effect(getTypeString(), params)
{
	filterManager->componentSelector.allowedComponents.removeAllInstancesOf(ComponentType::COLOR);
	filterManager->componentSelector.selectedComponents.set(ComponentType::INTENSITY, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::PANTILT, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::SERVO, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::STEPPER, true);

	smoothing = effectParams.addFloatParameter("Smoothing", "Smoothing intensity, higher value means smoother result", .5f, 0, 1);
	fallSmoothing = effectParams.addFloatParameter("Fall Smoothing", "Smoothing intensity, higher value means smoother result", .5f, 0, 1, false);
	fallSmoothing->canBeDisabledByUser = true;
}

SmoothingEffect::~SmoothingEffect()
{
}

void SmoothingEffect::onContainerTriggerTriggered(Trigger* t)
{
	Effect::onContainerTriggerTriggered(t);
	if (t == reset) prevValues.clear();
}

void SmoothingEffect::onContainerParameterChangedInternal(Parameter* p)
{
	Effect::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue()) prevValues.clear();
	}
	else if (p == weight)
	{
		if (weight->floatValue() == 0) prevValues.clear();
	}
}

var SmoothingEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{
	if (!prevValues.contains(c)) prevValues.set(c, values.clone());

	double t = time == -1 ? Time::getMillisecondCounterHiRes() / 1000.0 : time;
	if (!prevTimes.contains(c)) prevTimes.set(c, t);

	double deltaTime = t - prevTimes[c];

	if (deltaTime == 0) return values;

	var targetValues = values.clone();
	var curPrevValues = prevValues[c];

	float smoothVal = GetLinkedValue(smoothing);
	float fallSmoothVal = GetLinkedValue(fallSmoothing);

	for (int i = 0; i < values.size(); i++)
	{
		if (targetValues[i].isArray())
		{
			for (int j = 0; j < targetValues.size(); j++)
			{
				float pVal = (float)curPrevValues[i][j];
				float diff = (float)values[i][j] - pVal;
				float fac = (diff > 0 || !fallSmoothing->enabled) ? smoothVal : fallSmoothVal;
				if (fac > 0) targetValues[i][j] = pVal + diff * deltaTime / fac;
			}
		}
		else
		{
			float pVal = (float)curPrevValues[i];
			float diff = (float)values[i] - pVal;
			float fac = (diff > 0 || !fallSmoothing->enabled) ? smoothVal : fallSmoothVal;
			if (fac > 0) targetValues[i] = pVal + diff * deltaTime / fac;
		}
	}

	prevValues.set(c, targetValues.clone());
	prevTimes.set(c, t);

	return targetValues;
}