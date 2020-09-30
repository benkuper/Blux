/*
  ==============================================================================

    EffectManager.cpp
    Created: 30 Sep 2020 12:38:06pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectManager.h"

#include "effects/noise/NoiseEffect.h"

EffectManager::EffectManager() :
    BaseManager("Effects")
{
    itemDataType = "Effect";

    managerFactory = &factory;
    
    factory.defs.add(Factory<Effect>::Definition::createDef("", "Noise", &NoiseEffect::create));
}

EffectManager::~EffectManager()
{
}

void EffectManager::processComponentValues(ObjectComponent* c, var& values)
{
    for (auto& e: items)
    {
        if (!e->enabled->boolValue()) continue;
        e->processComponentValues(c, values);
    }
}
