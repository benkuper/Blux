/*
  ==============================================================================

    EffectManager.h
    Created: 30 Sep 2020 12:38:06pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

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
    EffectManager(Group * g = nullptr);
    ~EffectManager();

    Group* parentGroup;

    bool forceDisabled;

    void setForceDisabled(bool value);

    void addItemInternal(Effect* e, var data) override;
    void addItemsInternal(Array<Effect*> items, var data) override;

    virtual void processComponent(Object * o, ObjectComponent * c, HashMap<Parameter*, var> &values, float weightMultiplier = 1.0f, int id = -1);
    Array<ChainVizTarget *> getChainVizTargetsForObjectAndComponent(Object* o, ComponentType t);

    void resetEffectsTimes();

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

};