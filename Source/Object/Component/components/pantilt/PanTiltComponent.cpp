/*
  ==============================================================================

    PanTiltComponent.cpp
    Created: 26 Sep 2020 2:21:02pm
    Author:  bkupe

  ==============================================================================
*/

PanTiltComponent::PanTiltComponent(Object* o, var params) :
    ObjectComponent(o, getTypeString(), PANTILT, params)
{
    pan = addFloatParameter("Pan", "Pan", 0, 0, 1);
    tilt = addFloatParameter("Tilt", "Titlt", 0, 0, 1);

    sourceParameters.add(pan);
    sourceParameters.add(tilt);

    addComputedParameter(new FloatParameter("Pan", "Computed pan after all effects applied", 0, 0, 1), 0, pan);
    addComputedParameter(new FloatParameter("Tilt", "Computed tilt after all effects applied", 0, 0, 1), 1, tilt);
}

PanTiltComponent::~PanTiltComponent()
{
}
