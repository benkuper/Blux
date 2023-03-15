/*
  ==============================================================================

	OrientationEffect.cpp
	Created: 13 Mar 2023 4:56:10pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"
#include "Common/CommonIncludes.h"

#define FNL_IMPL
#include "Common/Helpers/FastNoiseLite.h"

OrientationTargetEffect::OrientationTargetEffect(var params) :
	Effect(getTypeString(), params)
{
	shape = effectParams.addEnumParameter("Shape", "Shape for multi device effects");
	shape->addOption("Point", POINT)->addOption("Line", LINE)->addOption("Circle", CIRCLE);

	count = effectParams.addIntParameter("Count", "Number of objects to consider for the effect", 1, 1);


	updateEffectParameters();
}

OrientationTargetEffect::~OrientationTargetEffect()
{
}

void OrientationTargetEffect::updateEffectParameters()
{
	GenericScopedLock lock(paramLock);

	for (auto& v : params) effectParams.removeControllable(v);
	params.clear();

	TargetShape s = shape->getValueDataAsEnum<TargetShape>();

	switch (s)
	{
	case POINT:
		params.add(effectParams.addPoint3DParameter("Position", "Position for the target"));
		break;

	case LINE:
		params.add(effectParams.addPoint3DParameter("Start", "Start of the line"));
		params.add(effectParams.addPoint3DParameter("End", "End of the line"));
		break;

	case CIRCLE:
		params.add(effectParams.addPoint3DParameter("Center", "Center of the circle"));
		Point3DParameter* op = effectParams.addPoint3DParameter("Orientation", "Orientation of the circle");
		op->setVector(1, 0, 0);
		params.add(op);
		params.add(effectParams.addFloatParameter("Radius", "Radius of the circle", 1));
		params.add(effectParams.addFloatParameter("Offset Angle", "Offset angle of the circle", 0));
		params.add(effectParams.addFloatParameter("Start Angle", "Start angle of the circle to draw partial shape", 0, 0, 1));
		params.add(effectParams.addFloatParameter("End Angle", "End angle of the circle", 1, 0, 1));
		params.add(effectParams.addFloatParameter("Turn Speed", "Speed of the circle", 0));
		break;
	}

}

void OrientationTargetEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	OrientationComponent* oc = dynamic_cast<OrientationComponent*>(c);
	if (oc == nullptr) return;

	GenericScopedLock lock(paramLock);

	int numObjects = GetLinkedValue(count);

	TargetShape s = shape->getValueDataAsEnum<TargetShape>();
	switch (s)
	{
	case POINT:
	{
		var pos = GetLinkedValue(params[0]);
		targetValues.set(oc->paramComputedMap[oc->target], pos);
	}
	break;

	case LINE:
	{
		jassert(params.size() >= 2);
		var start = GetLinkedValue(params[0]);
		var end = GetLinkedValue(params[1]);
		float relPos = id * 1.0f / jmax(numObjects - 1, 1);

		var v;
		for (int i = 0; i < start.size(); i++)
		{
			v.append(jmap<float>(relPos, start[i], end[i]));

		}
		targetValues.set(oc->paramComputedMap[oc->target], v);
	}
	break;

	case CIRCLE:
	{
		jassert(params.size() >= 6);
		var centerV = GetLinkedValue(params[0]);
		var orientationV = GetLinkedValue(params[1]);
		float radius = GetLinkedValue(params[2]);
		float offset = GetLinkedValue(params[3]);
		float start = GetLinkedValue(params[4]);
		float end = GetLinkedValue(params[5]);
		float turnSpeed = GetLinkedValue(params[6]);

		Vector3D<float> center(centerV[0], centerV[1], centerV[2]);



		// Calculate points on circle
		float relPos = id * 1.0f / numObjects;
		relPos = jmap(relPos, start, end);
		relPos = fmodf(relPos + offset + time * turnSpeed, 1);

		float angle = 2 * MathConstants<float>::pi * relPos; // Calculate angle
		float px = cos(angle) * radius;
		float pz = sin(angle) * radius;

		Vector3D<float> radRot = Vector3D<float>(degreesToRadians((float)orientationV[0]), degreesToRadians((float)orientationV[1]), degreesToRadians((float)orientationV[2]));
		Quat q = Quat::FromEuler(radRot);
		Vec3 p = q * Vec3(px, 0, pz) + center;

		var v;
		v.append(p.X);
		v.append(p.Y);
		v.append(p.Z);
		targetValues.set(oc->paramComputedMap[oc->target], v);
	}
	break;
	}
}

void OrientationTargetEffect::effectParamChanged(Controllable* c)
{
	Effect::effectParamChanged(c);

	if (c == shape)
	{
		updateEffectParameters();
	}
}

OrientationTargetNoiseEffect::OrientationTargetNoiseEffect(var params) :
	TimedEffect(getTypeString(), params)
{
	noiseType = effectParams.addEnumParameter("Noise Type", "Type of noise");
	noiseType->addOption("Simplex", SIMPLEX)->addOption("Perlin", PERLIN)->addOption("Sine", SINE);

	intensity = effectParams.addFloatParameter("Intensity", "Intensity", 1, 0);
	scale = effectParams.addFloatParameter("Scale", "Scale", 1, 0);

	axisMultiplier = effectParams.addPoint3DParameter("Axis Multiplier", "Multiply intensity along certain axis");
	axisMultiplier->setVector(1, 1, 1);

	range = effectParams.addPoint2DParameter("Range", "Min and max of this noise");
	range->setPoint(0, 1);
}

OrientationTargetNoiseEffect::~OrientationTargetNoiseEffect()
{
}

void OrientationTargetNoiseEffect::processedComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time, float originalTime)
{

	OrientationComponent* oc = dynamic_cast<OrientationComponent*>(c);
	if (oc == nullptr) return;

	var originalVal = values[oc->paramComputedMap[oc->target]];

	NoiseType t = noiseType->getValueDataAsEnum<NoiseType>();
	switch (t)
	{
	case SINE:

		break;

	case SIMPLEX:
	case PERLIN:
	{
		var intens = GetLinkedValue(intensity);
		var axisMult = GetLinkedValue(axisMultiplier);
		var rangeVal = GetLinkedValue(range);

		fnl_state noise = fnlCreateState();
		noise.noise_type = t == SIMPLEX ? FNL_NOISE_OPENSIMPLEX2 : FNL_NOISE_PERLIN;
		noise.frequency = GetLinkedValue(scale);

		var v;
		for (int i = 0; i < originalVal.size(); i++)
		{
			float aVal = fnlGetNoise2D(&noise, time, originalVal[i]);
			aVal = jmap<float>(aVal, rangeVal[0], rangeVal[1]);
			aVal *= (float)intens * (float)axisMult[i];
			v.append(aVal);
		}

		targetValues.set(oc->paramComputedMap[oc->target], v);
	}

	break;
	}
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