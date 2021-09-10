/*
  ==============================================================================

    ServoComponent.h
    Created: 26 Sep 2020 2:21:12pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ServoComponent :
    public ObjectComponent
{
public:
    ServoComponent(Object* o, var params);
    ~ServoComponent();

    String getTypeString() const override { return "Servo"; }
    static ServoComponent* create(Object * o, var params) { return new ServoComponent(o, params); }
};