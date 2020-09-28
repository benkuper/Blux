/*
  ==============================================================================

    GoboComponent.h
    Created: 26 Sep 2020 2:20:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class GoboComponent :
    public ObjectComponent
{
public:
    GoboComponent(var params);
    ~GoboComponent();

    String getTypeString() const override { return "Gobo"; }
    static GoboComponent* create(var params) { return new GoboComponent(params); }
};