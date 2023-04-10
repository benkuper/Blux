/*
  ==============================================================================

	CustomComponentEffect.cpp
	Created: 20 Mar 2023 8:51:42am
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

CustomComponentEffect::CustomComponentEffect(var params) :
	Effect(getTypeString(), params)
{
	numValues = effectParams.addIntParameter("Num Values", "Number of values", 3, 1);
	rebuildValues();
}

CustomComponentEffect::~CustomComponentEffect()
{
}

void CustomComponentEffect::rebuildValues()
{
	GenericScopedLock lock(valuesLock);
	while (customValues.size() > numValues->intValue())
	{
		effectParams.removeControllable(customValues.getLast());
		customValues.removeLast();
	}

	while (customValues.size() < numValues->intValue())
	{
		FloatParameter* fp = effectParams.addFloatParameter("Value " + String(customValues.size()), "Target position for local ID " + String(customValues.size()), 0, 0, 1, false);
		fp->canBeDisabledByUser = true;
		customValues.add(fp);
	}
}

void CustomComponentEffect::effectParamChanged(Controllable* p)
{
	if (p == numValues) rebuildValues();
}


void CustomComponentEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	CustomComponent* cp = dynamic_cast<CustomComponent*>(c);
	if (cp == nullptr) return;

	GenericScopedLock lock(valuesLock);

	for (int i = 0; i < customValues.size() && i < cp->numValues->intValue(); i++)
	{
		if (!customValues[i]->enabled) continue;
		var val = GetLinkedValue(customValues[i]);
		targetValues.set(cp->computedParameters[i], val);
	}
}

