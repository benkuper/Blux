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

class EffectFactory :
    public Factory<Effect>
{
public:
    juce_DeclareSingleton(EffectFactory, true)
    EffectFactory();
};

class EffectManager :
    public BaseManager<Effect>
{
public:
    EffectManager();
    ~EffectManager();


    virtual void processComponentValues(Object * o, ObjectComponent * c, var &values, float weightMultiplier = 1.0f);

    void resetEffectsTimes();

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

};