/*
  ==============================================================================

	OrientationTiltComponent.h
	Created: 26 Sep 2020 2:21:02pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#pragma warning(push)
#pragma warning(disable: 4201)

#include "Quaternion.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Matrix3x3.hpp"
typedef  unityports::Quaternion Quat;
typedef  unityports::Vector2 Vec2;
typedef  unityports::Vector3 Vec3;
typedef  unityports::Matrix3x3 Mat3;

class OrientationComponent :
	public ObjectComponent
{
public:
	OrientationComponent(Object* o, var params);
	~OrientationComponent();

	enum ControlMode { TARGET, PANTILT };
	EnumParameter* controlMode;
	Point3DParameter* target;
	FloatParameter* panOffset;
	FloatParameter* tiltOffset;

	EnablingControllableContainer panTiltCC;
	BoolParameter* usePreciseChannels;
	FloatParameter* pan;
	FloatParameter* tilt;


	FloatParameter* headOffset; //up offset from the base
	Point2DParameter* dmxPanValueRange; //min and max to do 180% on pan
	Point2DParameter* dmxTiltValueRange; //min and max to do 180% on tilt

	BoolParameter* useAlternativeRange;
	enum Axis { None, X, Y, Z };
	EnumParameter* alternativeAxis;

	Point2DParameter* dmxPanValueRange2; //min and max to do 180% on pan
	Point2DParameter* dmxTiltValueRange2; //min and max to do 180% on tilt

	Point3DParameter* debugPos;



	void setPanTiltFromTarget(Vec3 worldTarget);
	Vec3 inverseTransformPoint(Vec3 localPos, Vec3 localRot, Vec3 localScale, Vec3 targetPos);

	void onContainerParameterChangedInternal(Parameter*) override;
	bool checkDefaultInterfaceParamEnabled(Parameter* p) override { return p == pan || p == tilt; }

	void updateComputedValues(HashMap<Parameter*, var>& values) override;
	void fillInterfaceData(Interface* i, var data, var params) override;

	var getMappedValueForComputedParam(Interface* i, Parameter* cp) override;

	String getTypeString() const override { return "Orientation"; }
	static OrientationComponent* create(Object* o, var params) { return new OrientationComponent(o, params); }
};

#pragma warning(pop)