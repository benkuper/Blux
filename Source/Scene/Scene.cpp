/*
  ==============================================================================

	Scene.cpp
	Created: 26 Sep 2020 1:50:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Scene.h"
#include "Object/ObjectManager.h"
#include "Group/GroupManager.h"
#include "Effect/GlobalEffectManager.h"
#include "Engine/BluxEngine.h"
#include "Object//Component/components/intensity/IntensityComponent.h"
#include "Effect/effects/time/TimedEffect.h"

Scene::Scene(const String& name) :
	BaseItem(name, false),
	interpolationCurve("Loading Curve"),
	loadActions("Actions on Load"),
	unloadActions("Actions on Exit")
{
	saveAndLoadRecursiveData = true;

	saveTrigger = addTrigger("Save", "Save the current state of things into this scene");
	loadTrigger = addTrigger("Load", "Load this scene. This will change all parameters to what has been saved in this scene");
	defaultLoadTime = addFloatParameter("Load Time", "Default load time, used when using the \"Load\" trigger.", BluxSettings::getInstance()->defaultSceneLoadTime->floatValue(), 0);
	defaultLoadTime->defaultUI = FloatParameter::TIME;

	loadProgress = addFloatParameter("Load Progress", "Progress of the scene loading", 0, 0, 1);
	loadProgress->setControllableFeedbackOnly(true);

	isCurrent = addBoolParameter("Is Current", "Is this scene the current one ?", false);
	isCurrent->setControllableFeedbackOnly(true);

	saveScene(); //is this good ?

	interpolationCurve.isSelectable = false;
	interpolationCurve.length->setValue(1);
	interpolationCurve.addKey(0, 0, false);
	interpolationCurve.items[0]->easingType->setValueWithData(Easing::BEZIER);
	interpolationCurve.addKey(1, 1, false);
	interpolationCurve.selectItemWhenCreated = false;
	addChildControllableContainer(&interpolationCurve);

	sequenceManager.selectItemWhenCreated = false;
	sequenceManager.addBaseManagerListener(this);

	addChildControllableContainer(&effectManager);
	addChildControllableContainer(&sequenceManager);
	addChildControllableContainer(&loadActions);
	addChildControllableContainer(&unloadActions);

	effectManager.addBaseManagerListener(this);
}

Scene::~Scene()
{
}

void Scene::saveScene()
{
	sceneData = getSceneData();
	SystemClipboard::copyTextToClipboard(JSON::toString(sceneData));
	NLOG(niceName, "Scene saved");
}

var Scene::getSceneData()
{
	var result = var(new DynamicObject());
	result.getDynamicObject()->setProperty(ObjectManager::getInstance()->shortName, ObjectManager::getInstance()->getSceneData());
	result.getDynamicObject()->setProperty(GroupManager::getInstance()->shortName, GroupManager::getInstance()->getSceneData());
	result.getDynamicObject()->setProperty(GlobalEffectManager::getInstance()->shortName, GlobalEffectManager::getInstance()->getSceneData());
	return result;
}

void Scene::updateSceneData()
{
	ObjectManager::getInstance()->updateSceneData(sceneData);
	GroupManager::getInstance()->updateSceneData(sceneData);
	GlobalEffectManager::getInstance()->updateSceneData(sceneData);
}

void Scene::loadScene(float loadTime)
{
	sceneListeners.call(&SceneListener::askForLoadScene, this, loadTime);
}

bool Scene::isObjectActiveInScene(Object* o)
{
	float intensity = sceneData
		.getProperty(ObjectManager::getInstance()->shortName, var())
		.getProperty(o->shortName, var())
		.getProperty(o->componentManager.shortName, var())
		.getProperty("intensity", var())
		.getProperty("value", 0);

	bool result = intensity > 0;

	for (auto& e : effectManager.items)
	{
		if (e->filterManager.getFilteredResultForComponent(o, o->getComponent<IntensityComponent>()).id != -1)
		{
			result = true;
			break;
		}
	}

	return result;
}

void Scene::onContainerTriggerTriggered(Trigger* t)
{
	BaseItem::onContainerTriggerTriggered(t);

	if (t == saveTrigger) saveScene();
	else if (t == loadTrigger) loadScene();
}

void Scene::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == isCurrent)
	{
		if (isCurrent->boolValue()) loadActions.triggerAll();
		else unloadActions.triggerAll();
	}
}

void Scene::itemAdded(Sequence* s)
{
	s->startAtLoad->setValue(true);
	((BluxSequence*)s)->manualStartAtLoad = true;
}

void Scene::resetEffectTimes()
{
	effectManager.resetEffectsTimes();
}

void Scene::itemAdded(Effect* e)
{
	e->sceneSaveMode->hideInEditor = true;
}

var Scene::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("sceneData", sceneData);
	return data;
}

void Scene::loadJSONDataItemInternal(var data)
{
	sceneData = data.getDynamicObject()->getProperty("sceneData");
}
