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

Scene::Scene(const String& name) :
    BaseItem(name, false),
    interpolationCurve("Loading Curve")
{
    saveTrigger = addTrigger("Save","Save the current state of things into this scene");
    loadTrigger = addTrigger("Load", "Load this scene. This will change all parameters to what has been saved in this scene");
    defaultLoadTime = addFloatParameter("Load Time", "Default load time, used when using the \"Load\" trigger.", 0, 0);
    defaultLoadTime->defaultUI = FloatParameter::TIME;
    loadProgress = addFloatParameter("Load Progress", "Progress of the scene loading", 1, 0, 1);
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

    addChildControllableContainer(&sequenceManager);
}

Scene::~Scene()
{
}

void Scene::saveScene()
{
    sceneData = getSceneData();
    int numSavedData = sceneData.getDynamicObject()->getProperties().size();
    NLOG(niceName, "Scene saved with " << numSavedData << " parameters");
}

var Scene::getSceneData()
{
    var result = var(new DynamicObject());
    ObjectManager::getInstance()->saveSceneData(result);
    GroupManager::getInstance()->saveSceneData(result);
    GlobalEffectManager::getInstance()->saveSceneData(result);
    SystemClipboard::copyTextToClipboard(JSON::toString(result));
    return result;
}

void Scene::updateScene()
{
}

void Scene::loadScene()
{
    sceneListeners.call(&SceneListener::askForLoadScene, this);
}

void Scene::onContainerTriggerTriggered(Trigger* t)
{
    BaseItem::onContainerTriggerTriggered(t);
   
    if (t == saveTrigger) saveScene();
    else if (t == loadTrigger) loadScene();
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
