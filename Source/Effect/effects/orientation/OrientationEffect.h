/*
  ==============================================================================

	OrientationEffect.h
	Created: 13 Mar 2023 4:56:10pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class OrientationTargetEffect :
	public TimedEffect
{
public:
	OrientationTargetEffect(var params = var());
	virtual ~OrientationTargetEffect();

	enum TargetShape { POINT, LINE, CIRCLE };
	EnumParameter* shape;

	IntParameter* count;
	Array<Parameter*> params;

	SpinLock paramLock;

	void updateEffectParameters();

	void processComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1, float originalTime = -1) override;

	void effectParamChanged(Controllable* c) override;

	DECLARE_TYPE("Orientation Target")

};

class OrientationMultiTargetffect :
	public TimedEffect
{
public:
	OrientationMultiTargetffect(var params = var());
	virtual ~OrientationMultiTargetffect();

	IntParameter* numPositions;
	BoolParameter* loop;
	Array<Parameter*> positions;

	SpinLock posLock;
	
	void rebuildPositions();

	void effectParamChanged(Controllable* p) override;

	void processComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1, float originalTime = -1) override;

	DECLARE_TYPE("Orientation MultiTarget")

};

class OrientationTargetNoiseEffect :
	public TimedEffect
{
public:
	OrientationTargetNoiseEffect(var params = var());
	virtual ~OrientationTargetNoiseEffect();

	enum NoiseType { SIMPLEX, PERLIN, SINE };
	EnumParameter* noiseType;
	FloatParameter* intensity;
	FloatParameter* scale;
	Point3DParameter* axisMultiplier;
	Point2DParameter* range;

	void processComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1, float originalTime = -1) override;

	DECLARE_TYPE("Orientation Noise")

};

class OrientationPanTiltEffect :
	public Effect
{
public:
	OrientationPanTiltEffect(var params = var());
	virtual ~OrientationPanTiltEffect();

	FloatParameter* pan;
	FloatParameter* tilt;

	void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

	DECLARE_TYPE("Orientation PanTilt")
};
