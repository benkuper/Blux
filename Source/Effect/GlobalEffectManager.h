/*
  ==============================================================================

    GlobalEffectManager.h
    Created: 30 Sep 2020 12:38:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class EffectGroup :
    public BaseItem
{
public:
    EffectGroup();
    ~EffectGroup();

    EffectManager effectManager;

    BoolParameter* excludeFromScenes;

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    DECLARE_TYPE("EffectGroup");
};

class GlobalEffectManager :
    public BaseManager<EffectGroup>
{
public:
    juce_DeclareSingleton(GlobalEffectManager, true);

    GlobalEffectManager();
    ~GlobalEffectManager();

    Array<ChainVizTarget *> getChainVizTargetsForObjectAndComponent(Object* o, ComponentType t);
    virtual void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);
};