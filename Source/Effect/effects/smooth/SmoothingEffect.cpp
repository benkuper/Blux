/*
  ==============================================================================

	SmoothingEffect.cpp
	Created: 11 Sep 2021 5:16:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

SmoothingEffect::SmoothingEffect(var params) :
	Effect(getTypeString(), params)
{
	computePreviousValues = true;

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

void SmoothingEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{

	double t = time == -1 ? Time::getMillisecondCounterHiRes() / 1000.0 : time;
	if (!prevTimes.contains(c)) prevTimes.set(c, t);

	double deltaTime = t - prevTimes[c];

	if (deltaTime == 0) return;


	float smoothVal = GetLinkedValue(smoothing);
	float fallSmoothVal = GetLinkedValue(fallSmoothing);

	HashMap<Parameter*, var>::Iterator it(values);
	while(it.next())
	{
		Parameter* p = it.getKey();

		var prevVal = var();// prevValues[c][p]; //PROBLEM WITH NESTED HASHMAPs
		var val = it.getValue();
		jassert(prevVal.size() == val.size());

		var targetVal = val.clone();

		if (targetVal.isArray())
		{
			for (int i = 0; i < targetVal.size(); i++)
			{
				float diff = (float)val[i] - (float)prevVal[i];
				float fac = (diff > 0 || !fallSmoothing->enabled) ? smoothVal : fallSmoothVal;
				if (fac > 0) targetVal = (float)prevVal[i] + diff * deltaTime / fac;
			}
		}
		else
		{
			float diff = (float)val - (float)prevVal;
			float fac = (diff > 0 || !fallSmoothing->enabled) ? smoothVal : fallSmoothVal;
			if (fac > 0) targetVal = (float)prevVal + diff * deltaTime / fac;
		}

		targetValues.set(p, targetVal);
	}

	prevTimes.set(c, t);

}