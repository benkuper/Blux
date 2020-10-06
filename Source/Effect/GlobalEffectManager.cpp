/*
  ==============================================================================

    GlobalEffectManager.cpp
    Created: 30 Sep 2020 12:38:46pm
    Author:  bkupe

  ==============================================================================
*/

#include "GlobalEffectManager.h"

juce_ImplementSingleton(GlobalEffectManager)

GlobalEffectManager::GlobalEffectManager() :
    BaseManager("Global Effects")
{

}

GlobalEffectManager::~GlobalEffectManager()
{

}

void GlobalEffectManager::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    for (auto& i : items)
    {
        if (!i->enabled->boolValue()) continue;
        i->effectManager.processComponentValues(o, c, values);
    }
}

void GlobalEffectManager::saveSceneData(var& sceneData)
{
    for (auto& i : items) i->saveSceneData(sceneData);
}

EffectGroup::EffectGroup() :
    BaseItem("Effect Group")
{
    saveAndLoadRecursiveData = true;
    addChildControllableContainer(&effectManager);

    excludeFromScenes = addBoolParameter("Exclude From Scens", "If checked, this effect won't be saved in scens", false);
    excludeFromScenes->hideInEditor = true;
}

EffectGroup::~EffectGroup()
{
}

void EffectGroup::saveSceneData(var& sceneData)
{
    if (excludeFromScenes->boolValue()) return;

    sceneData.getDynamicObject()->setProperty(enabled->getControlAddress(), enabled->boolValue());
    effectManager.saveSceneData(sceneData);
}
