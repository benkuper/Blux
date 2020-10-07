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
#include "effects/automation/AutomationEffect.h"

EffectManager::EffectManager() :
    BaseManager("Effects")
{
    itemDataType = "Effect";

    managerFactory = &factory;
   
    factory.defs.add(Factory<Effect>::Definition::createDef("", "Override (Number)", &OverrideFloatEffect::create));
    factory.defs.add(Factory<Effect>::Definition::createDef("", "Override (Color)", &OverrideColorEffect::create));
    factory.defs.add(Factory<Effect>::Definition::createDef("", "Noise", &NoiseEffect::create));
    factory.defs.add(Factory<Effect>::Definition::createDef("", "Automation", &AutomationEffect::create));
}

EffectManager::~EffectManager()
{
}

void EffectManager::processComponentValues(Object * o, ObjectComponent* c, var& values, float weightMultiplier)
{
    for (auto& e: items)
    {
        if (!e->enabled->boolValue()) continue;
        e->processComponentValues(o, c, values, weightMultiplier);
    }
}

void EffectManager::saveSceneData(var &sceneData)
{
    var eData(new DynamicObject());
    for (auto& e : items) e->saveSceneData(sceneData);

}
