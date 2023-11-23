/*
  ==============================================================================

	PointEffect.cpp
	Created: 23 Nov 2023 4:02:06pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

PointEffect::PointEffect(const String& name, var params) :
	Effect(name, params),
	fadeCurve("Fade Curve")
{
	position = effectParams.addFloatParameter("Position", "Position of the point", 0);
	radius = effectParams.addFloatParameter("Radius", "Radius of the point", 2);
	
	if (!Engine::mainEngine->isLoadingFile)
	{
		fadeCurve.addKey(0, 1);
		fadeCurve.addKey(1, 0);
	}

	addChildControllableContainer(&fadeCurve);
}

PointEffect::~PointEffect()
{
}


PointFloatEffect::PointFloatEffect(var params) :
	PointEffect(getTypeString(), params)
{
	value = effectParams.addFloatParameter("Value", "Overriding value", 0, -1, 1);
	value->isCustomizableByUser = true;
}

PointFloatEffect::~PointFloatEffect()
{
}

void PointFloatEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	if (c->mainParameter == nullptr) return;
	jassert(!c->mainParameter->isComplex());

	float pos = GetLinkedValue(position);
	float rad = GetLinkedValue(radius);

	float relativeDist = jlimit<float>(0, 1, fabs(pos - id) / rad);
	float mapDist = fadeCurve.getValueAtPosition(relativeDist);
	
	float targetVal = GetLinkedValue(value);
	float val = targetVal * mapDist;
	targetValues.set(c->mainParameter, val);
}