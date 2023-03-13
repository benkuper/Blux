/*
  ==============================================================================

    OrientationTiltComponent.h
    Created: 26 Sep 2020 2:21:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class OrientationComponent :
    public ObjectComponent
{
public:
    OrientationComponent(Object* o, var params);
    ~OrientationComponent();

    Point3DParameter* target;

    EnablingControllableContainer panTiltCC;
    BoolParameter* usePreciseChannels;
    FloatParameter* pan;
    FloatParameter* tilt;

    String getTypeString() const override { return "Orientation"; }
    static OrientationComponent* create(Object * o, var params) { return new OrientationComponent(o, params); }
};