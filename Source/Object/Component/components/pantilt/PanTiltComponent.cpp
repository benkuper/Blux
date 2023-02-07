/*
  ==============================================================================

	PanTiltComponent.cpp
	Created: 26 Sep 2020 2:21:02pm
	Author:  bkupe

  ==============================================================================
*/

PanComponent::PanComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), PAN, params)
{
	pan = addFloatParameter("Value", "Pan", 0, 0, 1);

	sourceParameters.add(pan);

	addComputedParameter(new FloatParameter("Pan", "Computed pan after all effects applied", 0, 0, 1), 0, pan);
}

PanComponent::~PanComponent()
{
}

TiltComponent::TiltComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), TILT, params)
{
	tilt = addFloatParameter("Value", "Tilt", 0, 0, 1);

	sourceParameters.add(tilt);

	addComputedParameter(new FloatParameter("Tilt", "Computed tilt after all effects applied", 0, 0, 1), 0, tilt);
}

TiltComponent::~TiltComponent()
{
}

