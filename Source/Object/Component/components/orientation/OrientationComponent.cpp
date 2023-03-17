/*
  ==============================================================================

	OrientationTiltComponent.cpp
	Created: 26 Sep 2020 2:21:02pm
	Author:  bkupe

  ==============================================================================
*/
#include "Object/ObjectIncludes.h"
#include "Interface/InterfaceIncludes.h"
#include "Matrix3x3.hpp"
#include "Vector3.hpp"
#include "OrientationComponent.h"

OrientationComponent::OrientationComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), ORIENTATION, params),
	panTiltCC("Pan Tilt Control"),
	target(nullptr),
	pan(nullptr),
	tilt(nullptr)
{
	controlMode = addEnumParameter("Control Mode", "How to control this component. Target will set automatically pan and tilt, otherwise it will do nothing.");
	controlMode->addOption("Target", TARGET)->addOption("Pan/Tilt", PANTILT);

	target = (Point3DParameter*)addComputedParameter(new Point3DParameter("Target", "Orientation after all effects applied"));
	paramComputedMap[target]->setEnabled(false);

	usePreciseChannels = panTiltCC.addBoolParameter("Use Precise Channels", "If checked, this will also set the 2 precision channels. Depends on the device used.", false);
	pan = (FloatParameter*)addComputedParameter(new FloatParameter("Pan", "Pan", 0, 0, 1), &panTiltCC);
	tilt = (FloatParameter*)addComputedParameter(new FloatParameter("Tilt", "Tilt", 0, 0, 1), &panTiltCC);

	dmxPanValueRange = panTiltCC.addPoint2DParameter("DMX Pan Range", "Min and Max value for 360% pan rotation");
	dmxPanValueRange->setBounds(0, 0, 255, 255);
	dmxPanValueRange->setPoint(0, 255);
	dmxTiltValueRange = panTiltCC.addPoint2DParameter("DMX Tilt Range", "Min and Max value for 180% tilt rotation");
	dmxTiltValueRange->setBounds(0, 0, 255, 255);
	dmxTiltValueRange->setPoint(0, 255);

	useAlternativeRange = panTiltCC.addBoolParameter("Use Alternative Range", "Use the Range 2, allows to change where the pan break is", false);
	alternativeAxis = panTiltCC.addEnumParameter("Alternative Axis", "Axis to add 180% to when using alternative range");
	alternativeAxis->addOption("X", X)->addOption("Y", Y)->addOption("Z", Z)->addOption("None", None);

	dmxPanValueRange2 = panTiltCC.addPoint2DParameter("DMX Pan Range 2", "Alternative Min and Max value for 360% pan rotation");
	dmxPanValueRange2->setBounds(0, 0, 255, 255);
	dmxPanValueRange2->setPoint(0, 255);
	dmxTiltValueRange2 = panTiltCC.addPoint2DParameter("DMX Tilt Range 2", "Alternative Min and Max value for 180% tilt rotation");
	dmxTiltValueRange2->setBounds(0, 0, 255, 255);
	dmxTiltValueRange2->setPoint(0, 255);


	ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
	target->setEnabled(cm == TARGET);
	pan->setControllableFeedbackOnly(cm == TARGET);
	tilt->setControllableFeedbackOnly(cm == TARGET);

	headOffset = addFloatParameter("Head Offset", "Head distance to the base, to calculate target positionning", 0, 0);


	//debugPos = (Point3DParameter*)addComputedParameter(new Point3DParameter("Debug", "Debug Position"));
	addChildControllableContainer(&panTiltCC);
}

OrientationComponent::~OrientationComponent()
{

}

void OrientationComponent::setPanTiltFromTarget(Vec3 worldTarget)
{
	Vec3 position = Vec3::Up() * headOffset->floatValue();// InverseTransformPoint(transform.position, transform.rotation, transform.localScale, tiltT.position);

	Vec3 rot = object->stageRotation->getVector();

	if (useAlternativeRange->boolValue())
	{
		Axis ax = alternativeAxis->getValueDataAsEnum<Axis>();
		switch (ax)
		{
		case None: break;
		case X: rot.X += 180;
		case Y: rot.Y += 180;
		case Z: rot.Z += 180;
		}
	}


	Vec3 computedTarget = ((Point3DParameter*)paramComputedMap[target])->getVector();
	Vec3 localTarget = inverseTransformPoint(object->stagePosition->getVector(), rot, Vector3D<float>(1, 1, 1), computedTarget);

	//debugPos->setVector(localTarget.X, localTarget.Y, localTarget.Z);

	float targetTilt = 0;
	//Point<float> xzPos = new Point<float>(position.X, position.Z);
	Vec2 relXZTarget = Vec2(localTarget.X, localTarget.Z) - Vec2(position.X, position.Z);

	float xzAngle = Vec2::Angle(Vec2::Right(), relXZTarget);
	if (localTarget.Z < 0) xzAngle = -xzAngle;

	float xzDeg = radiansToDegrees(xzAngle);
	float targetPan = fmodf((-xzDeg + 90) / 360 + .5f, 1);
	//LOG("XZ Angle " << xzAngle << ", targetPan : " << targetPan);


	//Rotate around zero of the calculated pan to be able to simplify tilt computation to a XY angle
	Quat xzRot = Quat::FromEuler(0, xzAngle, 0);
	Vec3 cTarget = xzRot * localTarget;
	//debugPos->setVector(cTarget.X, cTarget.Y, cTarget.Z);



	Vec2 relXYTarget = Vec2(cTarget.X, cTarget.Y) - Vec2(position.X, position.Y);
	bool targetIsAccessible = relXYTarget.Y >= 0;

	if (targetIsAccessible)
	{
		float xyAngle = Vec2::Angle(Vec2::Right(), relXYTarget);
		float xyDeg = radiansToDegrees(xyAngle);
		targetTilt = xyDeg / 180;
		if (targetTilt > .5) targetTilt = 1 - targetTilt;


	}
	else
	{
		targetTilt = 0;
	}


	pan->setValue(targetPan);
	tilt->setValue(targetTilt);

}

Vec3 OrientationComponent::inverseTransformPoint(Vec3 localPos, Vec3 localRot, Vec3 localScale, Vec3 worldPos)
{

	Vec3 invScale(1.0 / localScale.X, 1.0 / localScale.Y, 1.0 / localScale.Z);

	Vec3 rotation(degreesToRadians(localRot.X), degreesToRadians(localRot.Y), degreesToRadians(localRot.Z));
	Mat3 rotMat = Mat3::FromQuaternion(Quat::FromEuler(rotation));

	Mat3 invRotation = Mat3::Transpose(rotMat);


	// Apply inverse scale, rotation, and translation to the world point
	return invRotation * (Vec3::Scale(worldPos, invScale) - localPos);
}

void OrientationComponent::onContainerParameterChangedInternal(Parameter* p)
{
	ObjectComponent::onContainerParameterChangedInternal(p);

	if (p == controlMode)
	{
		if (target != nullptr)
		{
			ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
			target->setEnabled(cm == TARGET);
			pan->setControllableFeedbackOnly(cm == TARGET);
			tilt->setControllableFeedbackOnly(cm == TARGET);
		}
	}
}

void OrientationComponent::updateComputedValues(HashMap<Parameter*, var>& values)
{
	if (!ObjectManager::getInstance()->blackOut->boolValue())
	{
		ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
		if (cm == TARGET)
		{
			target->setEnabled(cm == TARGET);

			Parameter* targetC = paramComputedMap[target];
			Parameter* panC = paramComputedMap[pan];
			Parameter* tiltC = paramComputedMap[tilt];

			var tVal = values[targetC];
			setPanTiltFromTarget(Vec3(tVal[0], tVal[1], tVal[1]));

			values.set(panC, pan->getValue());
			values.set(tiltC, tilt->getValue());
		}
	}

	ObjectComponent::updateComputedValues(values);
}

void OrientationComponent::fillInterfaceData(Interface* i, var data, var params)
{
	ObjectComponent::fillInterfaceData(i, data, params);

	if (ObjectManager::getInstance()->blackOut->boolValue()) return;

	if (DMXInterface* di = dynamic_cast<DMXInterface*>(i))
	{
		int channelOffset = params.getProperty("channelOffset", 0);
		var channelsData = data.getProperty("channels", var());

		if (usePreciseChannels->boolValue())
		{
			Array<Parameter*> panTilts{ pan, tilt };

			for (auto& p : panTilts)
			{

				Parameter* pCh = computedInterfaceMap[paramComputedMap[p]];
				if (pCh != nullptr || pCh->enabled)
				{
					int pChannel = channelOffset + pCh->intValue() - 1;

					float pVal = (float)channelsData[pChannel];
					int pVal1 = floor(pVal);
					int pVal2 = fmodf(pVal, 1) * 255;

					channelsData[pChannel] = pVal1;
					channelsData[pChannel+1] = pVal2;

				}
			}
		}
	}

}

var OrientationComponent::getMappedValueForComputedParam(Interface* i, Parameter* cp)
{
	if (DMXInterface* di = dynamic_cast<DMXInterface*>(i))
	{
		bool useAlt = useAlternativeRange->boolValue();
		Point2DParameter* panR = useAlt ? dmxPanValueRange2 : dmxPanValueRange;
		Point2DParameter* tiltR = useAlt ? dmxTiltValueRange2 : dmxTiltValueRange;

		if (cp == paramComputedMap[pan]) return jmap<float>(cp->floatValue(), panR->x, panR->y);
		else if (cp == paramComputedMap[tilt]) return jmap<float>(cp->floatValue(), tiltR->x, tiltR->y);
	}

	return ObjectComponent::getMappedValueForComputedParam(i, cp);
}
