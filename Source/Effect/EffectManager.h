/*
  ==============================================================================

    EffectManager.h
    Created: 30 Sep 2020 12:38:06pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Effect.h"

class Object;
class ObjectComponent;

class EffectManager :
    public BaseManager<Effect>
{
public:
    EffectManager();
    ~EffectManager();

    Factory<Effect> factory;

    virtual void processComponentValues(Object * o, ObjectComponent * c, var &values);
};