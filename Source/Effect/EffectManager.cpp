/*
  ==============================================================================

	EffectManager.cpp
	Created: 30 Sep 2020 12:38:06pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

juce_ImplementSingleton(EffectFactory);

EffectManager::EffectManager(Group* g) :
	BaseManager("Effects"),
	forceDisabled(false),
	parentGroup(g)
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
	e->setParentGroup(parentGroup);
}

void EffectManager::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier, int id)
{
	for (auto& e : items)
	{
		if (!e->enabled->boolValue()) continue;
		e->processComponent(o, c, values, weightMultiplier, id);
		if (c->componentType == ComponentType::DIMMER && values.size() > 0) o->effectIntensityOutMap.set(e, values[0].clone());
	}
}

Array<ChainVizTarget*> EffectManager::getChainVizTargetsForObject(Object* o)
{
	Array<ChainVizTarget*> result;
	for (auto& e : items) if (e->isAffectingObject(o)) result.add(e);
	return result;
}

void EffectManager::resetEffectsTimes()
{
	for (auto& e : items)
	{
		if (TimedEffect* te = dynamic_cast<TimedEffect*>(e)) te->resetTimes();
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
	defs.add(Factory<Effect>::Definition::createDef<OverrideFloatEffect>("Dimmer")->addParam("types", DIMMER));
	defs.add(Factory<Effect>::Definition::createDef<CurveMapEffect>("Dimmer")->addParam("types", DIMMER));
	defs.add(Factory<Effect>::Definition::createDef<NoiseEffect>("Dimmer")->addParam("types", DIMMER));
	defs.add(Factory<Effect>::Definition::createDef<AutomationEffect>("Dimmer")->addParam("types", DIMMER));
	defs.add(Factory<Effect>::Definition::createDef<FreezeFloatEffect>("Dimmer")->addParam("types", DIMMER));
	defs.add(Factory<Effect>::Definition::createDef<SmoothingEffect>("Dimmer")->addParam("types", DIMMER));

	defs.add(Factory<Effect>::Definition::createDef<OverrideFloatEffect>("Shutter")->addParam("types", SHUTTER));
	defs.add(Factory<Effect>::Definition::createDef<CurveMapEffect>("Shutter")->addParam("types", SHUTTER));
	defs.add(Factory<Effect>::Definition::createDef<NoiseEffect>("Shutter")->addParam("types", SHUTTER));
	defs.add(Factory<Effect>::Definition::createDef<AutomationEffect>("Shutter")->addParam("types", SHUTTER));
	defs.add(Factory<Effect>::Definition::createDef<FreezeFloatEffect>("Shutter")->addParam("types", SHUTTER));
	defs.add(Factory<Effect>::Definition::createDef<SmoothingEffect>("Shutter")->addParam("types", SHUTTER));

	defs.add(Factory<Effect>::Definition::createDef<ColorSourceOverrideEffect>("Color")->addParam("types", COLOR));
	defs.add(Factory<Effect>::Definition::createDef<HSVAdjustEffect>("Color")->addParam("types", COLOR));
	defs.add(Factory<Effect>::Definition::createDef<GradientRemapEffect>("Color")->addParam("types", COLOR));

	defs.add(Factory<Effect>::Definition::createDef<OrientationTargetEffect>("Orientation")->addParam("types", ORIENTATION));
	defs.add(Factory<Effect>::Definition::createDef<OrientationTargetNoiseEffect>("Orientation")->addParam("types", ORIENTATION));
	defs.add(Factory<Effect>::Definition::createDef<OrientationPanTiltEffect>("Orientation")->addParam("types", ORIENTATION));
}
