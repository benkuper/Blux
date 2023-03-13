/*
  ==============================================================================

    OrientationEffect.h
    Created: 13 Mar 2023 4:56:10pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class OrientationTargetEffect :
    public Effect
{
public:
    OrientationTargetEffect(var params = var());
    virtual ~OrientationTargetEffect();

    EnumParameter* shape;
    Point3DParameter* value;

    void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

    DECLARE_TYPE("Orientation Target")

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
