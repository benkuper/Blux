/*
  ==============================================================================

    ServoComponent.h
    Created: 26 Sep 2020 2:21:12pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class ServoComponent :
    public ObjectComponent
{
public:
    ServoComponent(var params);
    ~ServoComponent();

    String getTypeString() const override { return "Servo"; }
    static ServoComponent* create(var params) { return new ServoComponent(params); }
};