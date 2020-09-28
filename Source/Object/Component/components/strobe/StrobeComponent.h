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
    StrobeComponent(var params);
    ~StrobeComponent();

    String getTypeString() const override { return "Strobe"; }
    static StrobeComponent* create(var params) { return new StrobeComponent(params); }
};