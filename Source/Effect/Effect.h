/*
  ==============================================================================

    Effect.h
    Created: 30 Sep 2020 12:38:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Filter/FilterManager.h"
class ObjectComponent;

class Effect :
    public BaseItem
{
public:
    Effect(const String& name = "Effect", var params = var());
    ~Effect();

    FilterManager filterManager;

    virtual void processComponentValues(ObjectComponent* c, var& values) {}

    String getTypeString() const override { return "Effect"; }
};