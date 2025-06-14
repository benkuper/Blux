/*
  ==============================================================================

	Scene.cpp
	Created: 26 Sep 2020 1:50:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Scene/SceneIncludes.h"
#include "Engine/BluxEngine.h"

Scene::Scene(const String& name) :
	BaseItem(name, false),
	interpolationCurve("Loading Curve"),
	loadActions("Actions on Load"),
	unloadActions("Actions on Exit")
{
	saveAndLoadRecursiveData = true;

	saveTrigger = addTrigger("Save", "Save the current state of things into this scene");
	loadTrigger = addTrigger("Load", "Load this scene. This will change all parameters to what has been saved in this scene");
	loadTrigger->dashboardDefaultLabelParentLevel = 1;

	directLoadTrigger = addTrigger("Direct Load", "Directly load this scene. This will load without any timing");
	directLoadTrigger->dashboardDefaultLabelParentLevel = 1;
	directLoadTrigger->dashboardDefaultAppendLabel = true;

	defaultLoadTime = addFloatParameter("Load Time", "Default load time, used when using the \"Load\" trigger.", BluxSettings::getInstance()->defaultSceneLoadTime->floatValue(), 0);
	defaultLoadTime->dashboardDefaultLabelParentLevel = 1;
	defaultLoadTime->dashboardDefaultAppendLabel = true;
	defaultLoadTime->defaultUI = FloatParameter::TIME;

	loadProgress = addFloatParameter("Load Progress", "Progress of the scene loading", 0, 0, 1);
	loadProgress->setControllableFeedbackOnly(true);
	loadProgress->dashboardDefaultLabelParentLevel = 1;
	loadProgress->dashboardDefaultAppendLabel = true;

	isCurrent = addBoolParameter("Is Current", "Is this scene the current one ?", false);
	isCurrent->setControllableFeedbackOnly(true);
	isCurrent->isSavable = false;
	isCurrent->dashboardDefaultLabelParentLevel = 1;
	isCurrent->dashboardDefaultAppendLabel = true;

	if(!Engine::mainEngine->isLoadingFile) saveScene();

	interpolationCurve.isSelectable = false;
	interpolationCurve.length->setValue(1);
	interpolationCurve.addKey(0, 0, false);
	interpolationCurve.items[0]->easingType->setValueWithData(Easing::BEZIER);
	interpolationCurve.addKey(1, 1, false);
	interpolationCurve.selectItemWhenCreated = false;
	addChildControllableContainer(&interpolationCurve);

	sequenceManager.reset(new BluxSequenceManager());
	sequenceManager->selectItemWhenCreated = false;
	sequenceManager->addManagerListener(this);

	effectManager.reset(new EffectManager());

	addChildControllableContainer(sequenceManager.get());
	addChildControllableContainer(effectManager.get());
	addChildControllableContainer(&loadActions);
	addChildControllableContainer(&unloadActions);

	effectManager->addManagerListener(this);
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
		.getProperty(o->componentManager->shortName, var())
		.getProperty("intensity", var())
		.getProperty("value", 0);

	bool result = intensity > 0;

	for (auto& e : effectManager->items)
	{
		if (e->filterManager->getFilteredResultForComponent(o, o->getComponent<DimmerComponent>()).id != -1)
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
	else if (t == directLoadTrigger) loadScene(0);
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
	if (!isCurrentlyLoadingData) s->startAtLoad->setValue(true);
	((BluxSequence*)s)->manualStartAtLoad = true;
}

void Scene::itemsAdded(Array<Sequence*> items)
{
	for (auto& s : items)
	{
		if (!isCurrentlyLoadingData) s->startAtLoad->setValue(true);
		((BluxSequence*)s)->manualStartAtLoad = true;
	}
}


void Scene::resetEffectTimes()
{
	effectManager->resetEffectsTimes();
}

void Scene::itemAdded(Effect* e)
{
	e->sceneSaveMode->hideInEditor = true;
}

void Scene::itemsAdded(Array<Effect*> items)
{
	for (auto& e : items)
	{
		e->sceneSaveMode->hideInEditor = true;
	}
}


var Scene::getJSONData(bool includeNonOverriden)
{
	var data = BaseItem::getJSONData(includeNonOverriden);
	data.getDynamicObject()->setProperty("sceneData", sceneData);
	return data;
}

void Scene::loadJSONDataItemInternal(var data)
{
	sceneData = data.getDynamicObject()->getProperty("sceneData");
}
