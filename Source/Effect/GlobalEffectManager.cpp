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

EffectGroup::EffectGroup() :
    BaseItem("Effect Group")
{
    addChildControllableContainer(&effectManager);
}

EffectGroup::~EffectGroup()
{
}
