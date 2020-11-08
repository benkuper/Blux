/*
  ==============================================================================

    PanTiltComponent.h
    Created: 26 Sep 2020 2:21:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class PanTiltComponent :
    public ObjectComponent
{
public:
    PanTiltComponent(Object* o, var params);
    ~PanTiltComponent();

    FloatParameter* pan;
    FloatParameter* tilt;

    String getTypeString() const override { return "PanTilt"; }
    static PanTiltComponent* create(Object * o, var params) { return new PanTiltComponent(o, params); }
};