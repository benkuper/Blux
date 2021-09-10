/*
  ==============================================================================

    LaserComponent.h
    Created: 26 Sep 2020 2:20:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class LaserComponent :
    public ObjectComponent
{
public:
    LaserComponent(Object* o, var params);
    ~LaserComponent();

    String getTypeString() const override { return "Laser"; }
    static LaserComponent* create(Object * o, var params) { return new LaserComponent(o, params); }
};