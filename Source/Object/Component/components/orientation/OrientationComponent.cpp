/*
  ==============================================================================

	OrientationTiltComponent.cpp
	Created: 26 Sep 2020 2:21:02pm
	Author:  bkupe

  ==============================================================================
*/
#include "Object/ObjectIncludes.h"

OrientationComponent::OrientationComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), ORIENTATION, params),
	panTiltCC("Pan Tilt Control")
{
	target = (Point3DParameter*)addComputedParameter(new Point3DParameter("Target", "Orientation after all effects applied"));

	usePreciseChannels = panTiltCC.addBoolParameter("Use Precise Channels", "If checked, this will also set the 2 precision channels. Depends on the device used.", false);
	pan = (FloatParameter*)addComputedParameter(new FloatParameter("Pan", "Pan", 0, 0, 1), &panTiltCC);
	tilt = (FloatParameter*)addComputedParameter(new FloatParameter("Tilt", "Tilt", 0, 0, 1), &panTiltCC);

	addChildControllableContainer(&panTiltCC);

}

OrientationComponent::~OrientationComponent()
{
}