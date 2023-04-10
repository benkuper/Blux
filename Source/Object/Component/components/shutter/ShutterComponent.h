/*
  ==============================================================================

    ShutterComponent.h
    Created: 26 Sep 2020 2:21:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ShutterComponent :
    public ObjectComponent
{
public:
    ShutterComponent(Object* o, var params);
    ~ShutterComponent();

    String getTypeString() const override { return "Shutter"; }
    static ShutterComponent* create(Object * o, var params) { return new ShutterComponent(o, params); }
};