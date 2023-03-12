/*
  ==============================================================================

	OrientationTiltComponent.cpp
	Created: 26 Sep 2020 2:21:02pm
	Author:  bkupe

  ==============================================================================
*/
#include "Object/ObjectIncludes.h"

OrientationComponent::OrientationComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), ORIENTATION, params)
{
	target = addPoint3DParameter("Target", "The point to target in 3D");
	sourceParameters.add(target);

	addComputedParameter(new Point3DParameter("Target", "Computed orientation after all effects applied"), target);
}

OrientationComponent::~OrientationComponent()
{
}