/*
  ==============================================================================

	ComponentManager.cpp
	Created: 26 Sep 2020 10:02:54am
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"
#include "ComponentManager.h"

ComponentManager::ComponentManager(Object* o) :
	BaseManager("Components"),
	factory(o)
{
	managerFactory = &factory;
	selectItemWhenCreated = false;
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::addComponentFromDefinition(StringRef type, var definition, bool canBeRemoved)
{
	ObjectComponent* c = managerFactory->create(type);
	if (c == nullptr)
	{
		LOGWARNING("Component " << type << " doesn't exist.");
		return;
	}

	c->userCanRemove = canBeRemoved;
	c->setupFromJSONDefinition(definition);
	addItem(c);
}

var ComponentManager::getSceneData()
{
	var data(new DynamicObject());
	for (auto& c : items) data.getDynamicObject()->setProperty(c->shortName, c->getSceneData());
	return data;
}

void ComponentManager::updateSceneData(var& sceneData)
{
}

void ComponentManager::lerpFromSceneData(var startData, var endData, float weight)
{
	for (auto& i : items) i->lerpFromSceneData(startData.getProperty(i->shortName, var()), endData.getProperty(i->shortName, var()), weight);
}

var ComponentManager::getVizData()
{
	var data(new DynamicObject());
	for (auto& i : items) data.getDynamicObject()->setProperty(i->shortName, i->getVizData());
	return data;
}


//var ComponentManager::getJSONData(bool includeNonOverriden)
//{
//	var data = ControllableContainer::getJSONData(includeNonOverriden);
//	var cData;
//	for (auto& c : items) cData.append(c->getJSONData());
//	data.getDynamicObject()->setProperty("components", cData);
//
//	return data;
//}
//
//void ComponentManager::loadJSONDataInternal(var data)
//{
//	ControllableContainer::loadJSONDataInternal(data);
//
//	//Doing an index based loading, this may be improved but should be enough as long as objects don't change their components structure 
//	var cData = data.getProperty("components", var());
//	for (int i = 0; i < cData.size() && i < items.size(); i++) items[i]->loadJSONData(cData[i]);
//}




ComponentFactory::ComponentFactory(Object* o)
{
	defs.add(ComponentDefinition::createDef("", "Dimmer", &DimmerComponent::create, o));
	defs.add(ComponentDefinition::createDef("", "Color", &ColorComponent::create, o));
	//defs.add(ComponentDefinition::createDef("", "Shutter", &ShutterComponent::create, o));
	defs.add(ComponentDefinition::createDef("", "Orientation", &OrientationComponent::create, o));
	defs.add(ComponentDefinition::createDef("", "Custom", &CustomComponent::create, o));
}
