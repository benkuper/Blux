/*
  ==============================================================================

    EffectManager.cpp
    Created: 30 Sep 2020 12:38:06pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectManager.h"

#include "effects/time/noise/NoiseEffect.h"
#include "effects/override/OverrideEffect.h"
#include "effects/time/automation/AutomationEffect.h"

juce_ImplementSingleton(EffectFactory);

EffectManager::EffectManager() :
    BaseManager("Effects")
{
    itemDataType = "Effect";

    managerFactory = EffectFactory::getInstance();
   
    

    canBeCopiedAndPasted = true;
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

void EffectManager::resetEffectsTimes()
{
    for (auto& e : items)
    {
        if (TimedEffect * te = dynamic_cast<TimedEffect*>(e)) te->curTime = 0;
    }
}

var EffectManager::getSceneData()
{
    var data(new DynamicObject());
    for (auto& e : items) data.getDynamicObject()->setProperty(e->shortName, e->getSceneData());
    return data;
}

void EffectManager::updateSceneData(var& sceneData)
{
}

void EffectManager::lerpFromSceneData(var startData, var endData, float weight)
{
    for (auto& i : items) i->lerpFromSceneData(startData.getProperty(i->shortName, var()), endData.getProperty(i->shortName, var()), weight);
}



EffectFactory::EffectFactory()
{
    defs.add(Factory<Effect>::Definition::createDef("", OverrideFloatEffect::getTypeStringStatic(), &OverrideFloatEffect::create));
    defs.add(Factory<Effect>::Definition::createDef("", OverrideColorEffect::getTypeStringStatic(), &OverrideColorEffect::create));
    defs.add(Factory<Effect>::Definition::createDef("", NoiseEffect::getTypeStringStatic(), &NoiseEffect::create));
    defs.add(Factory<Effect>::Definition::createDef("", AutomationEffect::getTypeStringStatic(), &AutomationEffect::create));
}
