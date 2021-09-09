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
#include "Object/Object.h"
#include "effects/color/override/ColorSourceOverrideEffect.h"
#include "effects/color/hsv/HSVAdjustEffect.h"
#include "effects/color/gradientmap/ColorGradientMapEffect.h"

juce_ImplementSingleton(EffectFactory);

EffectManager::EffectManager() :
    BaseManager("Effects"),
    forceDisabled(false)
{
    itemDataType = "Effect";

    managerFactory = EffectFactory::getInstance();

    selectItemWhenCreated = false;
    canBeCopiedAndPasted = true;
}

EffectManager::~EffectManager()
{
}

void EffectManager::setForceDisabled(bool value)
{
    if (forceDisabled == value) return;
    forceDisabled = value;
    for (auto& i : items) i->setForceDisabled(forceDisabled);
}

void EffectManager::addItemInternal(Effect* e, var data)
{
    BaseManager::addItemInternal(e, data);
    e->setForceDisabled(forceDisabled);
}

void EffectManager::processComponentValues(Object * o, ObjectComponent* c, var& values, float weightMultiplier)
{
    for (auto& e: items)
    {
        if (!e->enabled->boolValue()) continue;
        e->processComponentValues(o, c, values, weightMultiplier);
        if (c->componentType == c->INTENSITY && values.size() > 0) o->effectIntensityOutMap.set(e, values[0].clone());
    }
}

Array<ChainVizTarget *> EffectManager::getChainVizTargetsForObject(Object* o)
{
    Array<ChainVizTarget *> result;
    for(auto & e: items) if(e->isAffectingObject(o)) result.add(e);
    return result;
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
    defs.add(Factory<Effect>::Definition::createDef("Number", OverrideFloatEffect::getTypeStringStatic(), &OverrideFloatEffect::create));
    defs.add(Factory<Effect>::Definition::createDef("Number", NoiseEffect::getTypeStringStatic(), &NoiseEffect::create));
    defs.add(Factory<Effect>::Definition::createDef("Number", AutomationEffect::getTypeStringStatic(), &AutomationEffect::create));

    defs.add(Factory<Effect>::Definition::createDef("Color", ColorSourceOverrideEffect::getTypeStringStatic(), &ColorSourceOverrideEffect::create));
    defs.add(Factory<Effect>::Definition::createDef("Color", HSVAdjustEffect::getTypeStringStatic(), &HSVAdjustEffect::create));
    defs.add(Factory<Effect>::Definition::createDef("Color", GradientRemapEffect::getTypeStringStatic(), &GradientRemapEffect::create));
}
