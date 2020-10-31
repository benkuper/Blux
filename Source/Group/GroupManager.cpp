/*
  ==============================================================================

    GroupManager.cpp
    Created: 29 Sep 2020 9:46:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "GroupManager.h"
#include "groups/object/ObjectGroup.h"
juce_ImplementSingleton(GroupManager)

GroupManager::GroupManager() :
    BaseManager("Groups")
{
    managerFactory = &factory;

    factory.defs.add(Factory<Group>::Definition::createDef("", "Object Group", &ObjectGroup::create));
}

GroupManager::~GroupManager()
{
}

Array<ChainVizTarget *> GroupManager::getChainVizTargetsForObject(Object* o)
{
    Array<ChainVizTarget *> result;
    for (auto& g : items)
    {
        if (!g->containsObject(o)) continue;
        result.addArray(g->effectManager.getChainVizTargetsForObject(o));
    }
    return result;
}

void GroupManager::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    for (auto& g : items)
    {
        if (!g->enabled->boolValue()) continue;
        if (g->containsObject(o)) g->processComponentValues(o,  c, values);
    }
}

var GroupManager::getSceneData()
{
    var data(new DynamicObject());
    for (auto& i : items) data.getDynamicObject()->setProperty(i->shortName, i->getSceneData());
    return data;
}

void GroupManager::updateSceneData(var& sceneData)
{
}

void GroupManager::lerpFromSceneData(var startData, var endData, float weight)
{
    for (auto& i : items) i->lerpFromSceneData(startData.getProperty(i->shortName, var()), endData.getProperty(i->shortName, var()), weight);
}
