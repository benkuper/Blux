/*
  ==============================================================================

	OrientationEffect.cpp
	Created: 13 Mar 2023 4:56:10pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

OrientationTargetEffect::OrientationTargetEffect(var params) :
	Effect(getTypeString(), params)
{
	shape = effectParams.addEnumParameter("Shape", "Shape for multi device effects");
	value = effectParams.addPoint3DParameter("Position", "Position for the target");
}

OrientationTargetEffect::~OrientationTargetEffect()
{
}

void OrientationTargetEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	OrientationComponent* oc = dynamic_cast<OrientationComponent*>(c);
	if (oc == nullptr) return;

	targetValues.set(oc->paramComputedMap[oc->target], GetLinkedValue(value));
}




OrientationPanTiltEffect::OrientationPanTiltEffect(var params) :
	Effect(getTypeString(), params)
{
	pan = effectParams.addFloatParameter("Pan", "Pan", 0, -1, 1);
	pan->canBeDisabledByUser = true;
	tilt = effectParams.addFloatParameter("Tilt", "Tilt", 0, -1, 1);
	tilt->canBeDisabledByUser = true;
}

OrientationPanTiltEffect::~OrientationPanTiltEffect()
{
}

void OrientationPanTiltEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	OrientationComponent* oc = dynamic_cast<OrientationComponent*>(c);
	if (oc == nullptr) return;

	Parameter* panCP = oc->paramComputedMap[oc->pan];
	Parameter* tiltCP = oc->paramComputedMap[oc->tilt];

	if (pan->enabled) targetValues.set(panCP, GetLinkedValue(pan));
	if (tilt->enabled) targetValues.set(tiltCP, GetLinkedValue(tilt));
}

