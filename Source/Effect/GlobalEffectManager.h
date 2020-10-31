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
    ComponentSelector componentSelector;

    BoolParameter* excludeFromScenes;

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    String getTypeString() const override { return "EffectGroup"; }
};

class GlobalEffectManager :
    public BaseManager<EffectGroup>
{
public:
    juce_DeclareSingleton(GlobalEffectManager, true);

    GlobalEffectManager();
    ~GlobalEffectManager();

    Array<ChainVizTarget *> getChainVizTargetsForObject(Object* o);
    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);
};