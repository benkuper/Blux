/*
  ==============================================================================

    LaserComponent.h
    Created: 26 Sep 2020 2:20:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class LaserComponent :
    public ObjectComponent
{
public:
    LaserComponent(var params);
    ~LaserComponent();

    String getTypeString() const override { return "Laser"; }
    static LaserComponent* create(var params) { return new LaserComponent(params); }
};