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
    Thread("SceneLoad"),
    interpolationCurve("Loading Curve")
{
    saveTrigger = addTrigger("Save","Save the current state of things into this scene");
    loadTrigger = addTrigger("Load", "Load this scene. This will change all parameters to what has been saved in this scene");
    defaultLoadTime = addFloatParameter("Load Time", "Default load time, used when using the \"Load\" trigger.", 0, 0);
    defaultLoadTime->defaultUI = FloatParameter::TIME;
    loadProgress = addFloatParameter("Load Progress", "Progress of the scene loading", 1, 0, 1);
    loadProgress->setControllableFeedbackOnly(true);
    saveScene(); //is this good ?

    interpolationCurve.isSelectable = false;
    interpolationCurve.length->setValue(1);
    interpolationCurve.addKey(0, 0, false);
    interpolationCurve.items[0]->easingType->setValueWithData(Easing::BEZIER);
    interpolationCurve.addKey(1, 1, false);
    interpolationCurve.selectItemWhenCreated = false;
    interpolationCurve.hideEditorHeader = true;
    addChildControllableContainer(&interpolationCurve);
}

Scene::~Scene()
{
    threadShouldExit();
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

void Scene::loadScene(float time)
{
    if (sceneData.isVoid())
    {
        NLOGWARNING(niceName, "Scene is empty, save at least once before loading");
        return;
    }

    stopThread(1000);

    if (time == -1) loadTime = time >= 0 ? time : defaultLoadTime->floatValue();

    startThread();

}

void Scene::updateScene()
{
}

void Scene::onContainerTriggerTriggered(Trigger* t)
{
    BaseItem::onContainerTriggerTriggered(t);
   
    if (t == saveTrigger) saveScene();
    else if (t == loadTrigger) loadScene();
}

void Scene::run()
{
    float timeAtLoad = Time::getMillisecondCounter() / 1000.0f;
    loadProgress->setValue(0);

    var rawDataAtLoad = getSceneData();

    dataAtLoad = var(new DynamicObject());
    dataToLoad = var(new DynamicObject());
    
    int numChanged = 0;
    NamedValueSet props = sceneData.getDynamicObject()->getProperties();
    for (auto& p : props)
    {
        var rawValue = rawDataAtLoad.getDynamicObject()->getProperty(p.name);
        if (rawValue != p.value)
        {
            dataAtLoad.getDynamicObject()->setProperty(p.name, rawValue);
            dataToLoad.getDynamicObject()->setProperty(p.name, p.value);
            numChanged++;
        }
    }

    NLOG(niceName, "Loading Scene, " << numChanged << " changed parameters.");

    if (loadTime == 0) lerpSceneParams(1);
    else
    {
        while (!threadShouldExit() && loadProgress->floatValue() < 1)
        {
            float curTime = Time::getMillisecondCounter() / 1000.0f;;
            float progress = (curTime - timeAtLoad) / loadTime;
            loadProgress->setValue(progress);
            lerpSceneParams(interpolationCurve.getValueAtPosition(progress));
            sleep(30);
        }
    }
  
    loadProgress->setValue(0);
}

void Scene::lerpSceneParams(float weight)
{
    NamedValueSet props = sceneData.getDynamicObject()->getProperties();
    for (auto& p : props)
    {
        
        if (Parameter* param = dynamic_cast<Parameter*>(Engine::mainEngine->getControllableForAddress(p.name.toString())))
        {
            var val = p.value.clone();
            var valueAtLoad = dataAtLoad.getProperty(p.name, val.clone());

            if (p.value.isArray())
            {
                for (int i = 0; i < p.value.size(); i++)
                {
                    val.append((float)valueAtLoad[i] + ((float)p.value[i] - (float)valueAtLoad[i]) * weight);
                }
            }
            else if (p.value.isDouble() || p.value.isInt())
            {
                val = (float)valueAtLoad + ((float)p.value - (float)valueAtLoad) * weight;
            }
            else if (p.value.isBool())
            {
                val = weight >= .5f ? valueAtLoad : p.value;
            }

            param->setValue(val);
        }
    }
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
