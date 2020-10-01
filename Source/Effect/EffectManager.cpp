/*
  ==============================================================================

    EffectManager.cpp
    Created: 30 Sep 2020 12:38:06pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectManager.h"

#include "effects/noise/NoiseEffect.h"
#include "effects/override/OverrideEffect.h"

EffectManager::EffectManager() :
    BaseManager("Effects")
{
    itemDataType = "Effect";

    managerFactory = &factory;
   
    factory.defs.add(Factory<Effect>::Definition::createDef("", "Override (Float)", &OverrideFloatEffect::create));
    factory.defs.add(Factory<Effect>::Definition::createDef("", "Noise", &NoiseEffect::create));
}

EffectManager::~EffectManager()
{
}

void EffectManager::processComponentValues(Object * o, ObjectComponent* c, var& values)
{
    for (auto& e: items)
    {
        if (!e->enabled->boolValue()) continue;
        e->processComponentValues(o, c, values);
    }
}
