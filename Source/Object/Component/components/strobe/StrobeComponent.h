/*
  ==============================================================================

    StrobeComponent.h
    Created: 26 Sep 2020 2:21:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class StrobeComponent :
    public ObjectComponent
{
public:
    StrobeComponent(Object* o, var params);
    ~StrobeComponent();

    String getTypeString() const override { return "Strobe"; }
    static StrobeComponent* create(Object * o, var params) { return new StrobeComponent(o, params); }
};