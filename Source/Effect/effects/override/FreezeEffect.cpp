/*
  ==============================================================================

	FreezeEffect.cpp
	Created: 11 Sep 2021 5:16:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

FreezeEffect::FreezeEffect(const String& name, var params) :
	Effect(name, params)
{
	computePreviousValues = true;

	freezeMode = effectParams.addEnumParameter("Mode", "Keep only higher values or lower values");
	freezeMode->addOption("Hold", HOLD)->addOption("Increase Only", MAX)->addOption("Decrease Only", MIN);

	reset = effectParams.addTrigger("Reset", "Reset the freeze");
}

FreezeEffect::~FreezeEffect()
{
}


void FreezeEffect::effectParamChanged(Controllable* c)
{
	Effect::effectParamChanged(c);
	if (c == reset) clearPrevValues();
}


void FreezeEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	processComponentFreezeInternal(o, c, values, targetValues, id, time);
}


FreezeFloatEffect::FreezeFloatEffect(var params) :
	FreezeEffect(getTypeString(), params)
{

}

FreezeFloatEffect::~FreezeFloatEffect()
{
}

void FreezeFloatEffect::processComponentFreezeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	FreezeMode m = freezeMode->getValueDataAsEnum<FreezeMode>();

	if (!prevValuesMap.contains(c)) return;

	if (m == HOLD)
	{
		HashMap<Parameter*, var>::Iterator it(*prevValuesMap[c]);
		while (it.next()) targetValues.set(it.getKey(), it.getValue());
		return;
	}

	HashMap<Parameter*, var>::Iterator it(values);
	while (it.next())
	{
		Parameter* p = it.getKey();
		var prevVal = (*prevValuesMap[c])[p];
		var val = it.getValue();
		jassert(prevVal.size() == val.size());

		var targetVal = val.clone();

		if (val.isArray())
		{
			for (int i = 0; i < values.size(); i++)
			{
				targetVal = m == MAX ? jmax((float)prevVal, (float)val[i]) : jmin((float)prevVal[i], (float)val[i]);
			}
		}
		else
		{
			targetVal = m == MAX ? jmax((float)prevVal, (float)val) : jmin((float)prevVal, (float)val);
		}

		targetValues.set(p, targetVal);
	}
}