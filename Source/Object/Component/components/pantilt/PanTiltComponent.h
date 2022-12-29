/*
  ==============================================================================

    PanTiltComponent.h
    Created: 26 Sep 2020 2:21:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class PanComponent :
    public ObjectComponent
{
public:
    PanComponent(Object* o, var params);
    ~PanComponent();

    FloatParameter* pan;

    String getTypeString() const override { return "Pan"; }
    static PanComponent* create(Object * o, var params) { return new PanComponent(o, params); }
};

class TiltComponent :
    public ObjectComponent
{
public:
    TiltComponent(Object* o, var params);
    ~TiltComponent();

    FloatParameter* tilt;

    String getTypeString() const override { return "Tilt"; }
    static TiltComponent* create(Object* o, var params) { return new TiltComponent(o, params); }
};