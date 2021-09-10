/*
  ==============================================================================

    GoboComponent.h
    Created: 26 Sep 2020 2:20:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class GoboComponent :
    public ObjectComponent
{
public:
    GoboComponent(Object * o, var params);
    ~GoboComponent();

    String getTypeString() const override { return "Gobo"; }
    static GoboComponent* create(Object * o, var params) { return new GoboComponent(o, params); }
};