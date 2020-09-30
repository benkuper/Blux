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

EffectGroup::EffectGroup() :
    BaseItem("Effect Group")
{
    saveAndLoadRecursiveData = true;
    addChildControllableContainer(&effectManager);
}

EffectGroup::~EffectGroup()
{
}
