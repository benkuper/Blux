/*
  ==============================================================================

    GlobalEffectManager.h
    Created: 30 Sep 2020 12:38:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "EffectManager.h"

class EffectGroup :
    public BaseItem
{
public:
    EffectGroup();
    ~EffectGroup();

    EffectManager effectManager;

    void saveSceneData(var& sceneData);

    String getTypeString() const override { return "EffectGroup"; }
};

class GlobalEffectManager :
    public BaseManager<EffectGroup>
{
public:
    juce_DeclareSingleton(GlobalEffectManager, true);

    GlobalEffectManager();
    ~GlobalEffectManager();

    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values);

    void saveSceneData(var& sceneData);

};