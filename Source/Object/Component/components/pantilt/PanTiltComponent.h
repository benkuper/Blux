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
    PanTiltComponent(var params);
    ~PanTiltComponent();

    FloatParameter* pan;
    FloatParameter* tilt;

    String getTypeString() const override { return "PanTilt"; }
    static PanTiltComponent* create(var params) { return new PanTiltComponent(params); }
};