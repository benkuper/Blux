/*
  ==============================================================================

	GlobalEffectManager.cpp
	Created: 30 Sep 2020 12:38:46pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

juce_ImplementSingleton(GlobalEffectManager)

GlobalEffectManager::GlobalEffectManager() :
	BaseManager("Global Effects")
{

}

GlobalEffectManager::~GlobalEffectManager()
{

}

Array<ChainVizTarget*> GlobalEffectManager::getChainVizTargetsForObjectAndComponent(Object* o, ComponentType c)
{
	Array<ChainVizTarget*> result;
	for (auto& g : items) result.addArray(g->effectManager.getChainVizTargetsForObjectAndComponent(o, c));
	return result;
}

void GlobalEffectManager::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values)
{
	for (auto& i : items)
	{
		if (!i->enabled->boolValue()) continue;
		i->effectManager.processComponent(o, c, values);
	}
}

var GlobalEffectManager::getSceneData()
{
	var data(new DynamicObject());
	for (auto& i : items) data.getDynamicObject()->setProperty(i->shortName, i->getSceneData());
	return data;
}

void GlobalEffectManager::updateSceneData(var& sceneData)
{
}

void GlobalEffectManager::lerpFromSceneData(var startData, var endData, float weight)
{
	for (auto& i : items) i->lerpFromSceneData(startData.getProperty(i->shortName, var()), endData.getProperty(i->shortName, var()), weight);
}

EffectGroup::EffectGroup() :
	BaseItem("Effect Group")
{
	saveAndLoadRecursiveData = true;
	addChildControllableContainer(&effectManager);

	excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked, this effect won't be saved in scenes", false);
}

EffectGroup::~EffectGroup()
{
}

var EffectGroup::getSceneData()
{
	var data(new DynamicObject());
	if (excludeFromScenes->boolValue()) return data;
	data.getDynamicObject()->setProperty(effectManager.shortName, effectManager.getSceneData());
	return data;
}

void EffectGroup::updateSceneData(var& sceneData)
{
}

void EffectGroup::lerpFromSceneData(var startData, var endData, float weight)
{
	if (excludeFromScenes->boolValue()) return;
	effectManager.lerpFromSceneData(startData.getProperty(effectManager.shortName, var()), endData.getProperty(effectManager.shortName, var()), weight);
}
