/*
  ==============================================================================

    SceneManager.cpp
    Created: 26 Sep 2020 1:49:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "SceneManager.h"

#include "Object/ObjectManager.h"
#include "Group/GroupManager.h"
#include "Effect/GlobalEffectManager.h"

juce_ImplementSingleton(SceneManager)

SceneManager::SceneManager() :
    BaseManager("Scenes"),
    Thread("Scene Load"),
    previousScene(nullptr),
    currentScene(nullptr)
{
    managerFactory = &factory;
    factory.defs.add(Factory<Scene>::Definition::createDef("", "Scene", &Scene::create));

    loadNextSceneTrigger = addTrigger("Load Next Scene", "Load the next scene. If no scene is loaded, will load first one");
    loadPreviousSceneTrigger = addTrigger("Load Previous Scene", "Load the previous scene. If no scene is loaded, this will do nothing.");

    autoPreview = addBoolParameter("Auto Preview", "If checked, this will force preview when hovering a scene",  false);
}

SceneManager::~SceneManager()
{
    threadShouldExit();
}


void SceneManager::addItemInternal(Scene* s, var data)
{
    s->addSceneListener(this);
}

void SceneManager::removeItemInternal(Scene* s)
{
    s->removeSceneListener(this);
}

void SceneManager::loadScene(Scene* s, float time)
{
    //if (s == currentScene) return;

    stopThread(1000);
   
    if (currentScene != nullptr)
    {
        currentScene->isCurrent->setValue(false);
        currentScene->loadProgress->setValue(0);
        currentScene->removeInspectableListener(this);
    }

    previousScene = currentScene;
    currentScene = s;

    if (currentScene == nullptr) return;

    if (currentScene->sceneData.isVoid())
    {
        NLOGWARNING(niceName, "Scene is empty, save at least once before loading");
        return;
    }

    currentScene->addInspectableListener(this);


    loadTime = time >= 0 ? time : currentScene->defaultLoadTime->floatValue();

    startThread();
}

void SceneManager::run()
{
    float timeAtLoad = Time::getMillisecondCounter() / 1000.0f;
    currentScene->loadProgress->setValue(0);

    var dataAtLoad = currentScene->getSceneData();
     
    /*
    dataAtLoad = var(new DynamicObject());
    dataToLoad = var(new DynamicObject());

    int numChanged = 0;
    NamedValueSet props = currentScene->sceneData.getDynamicObject()->getProperties();
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

    NLOG(niceName, "Loading Scene " << currentScene->niceName <<", " << numChanged << " changed parameters.");
    */

    String oName = ObjectManager::getInstance()->shortName;
    String gName = GroupManager::getInstance()->shortName;
    String eName = GlobalEffectManager::getInstance()->shortName;
    
    
    if (loadTime > 0)
    {
        while (!threadShouldExit() && currentScene->loadProgress->floatValue() < 1)
        {
            if (Engine::mainEngine->isClearing) return;

            float curTime = Time::getMillisecondCounter() / 1000.0f;;
            float progress = (curTime - timeAtLoad) / loadTime;
            currentScene->loadProgress->setValue(progress);

            float weight = currentScene->interpolationCurve.getValueAtPosition(progress);

            ObjectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(oName, var()), currentScene->sceneData.getProperty(oName, var()), weight);
            GroupManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(gName, var()), currentScene->sceneData.getProperty(gName, var()), weight);
            GlobalEffectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(eName, var()), currentScene->sceneData.getProperty(eName, var()), weight);

            sleep(30);
        }
    }

    if (Engine::mainEngine->isClearing) return;

    
    if (currentScene->loadProgress->floatValue() == 1)
    {
        ObjectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(oName, var()), currentScene->sceneData.getProperty(oName, var()), 1);
        GroupManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(gName, var()), currentScene->sceneData.getProperty(gName, var()), 1);
        GlobalEffectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(eName, var()), currentScene->sceneData.getProperty(eName, var()), 1);
        currentScene->isCurrent->setValue(true);
    }

    currentScene->loadProgress->setValue(0);
}

/*
void SceneManager::lerpSceneParams(float weight)
{
    NamedValueSet props = currentScene->sceneData.getDynamicObject()->getProperties();
    for (auto& p : props)
    {

        if (Parameter* param = dynamic_cast<Parameter*>(Engine::mainEngine->getControllableForAddress(p.name.toString())))
        {
            var val = var();// p.value.clone();
            var valueAtLoad = dataAtLoad.getProperty(p.name, p.value.clone());

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
*/

void SceneManager::askForLoadScene(Scene* s, float loadTime)
{
    loadScene(s, loadTime);
}

void SceneManager::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    if (currentScene == nullptr) return;

    float progressWeight = currentScene->isCurrent->boolValue() ? 1 : currentScene->loadProgress->floatValue();
    if (previousScene != nullptr &&  progressWeight < 1)  previousScene->effectManager.processComponentValues(o, c, values, 1 - currentScene->loadProgress->floatValue());
    currentScene->effectManager.processComponentValues(o, c, values,  progressWeight);
}

void SceneManager::onContainerTriggerTriggered(Trigger* t)
{
    if (t == loadNextSceneTrigger)
    {
        int index = items.indexOf(currentScene) + 1;
        if (index < items.size())
        {
            loadScene(items[index]);
        }
    }
    else if (t == loadPreviousSceneTrigger)
    {
        int index = items.indexOf(currentScene) - 1;
        if (index >= 0)
        {
            loadScene(items[index]);
        }
    }
}

void SceneManager::inspectableDestroyed(Inspectable* i)
{
    if (i == currentScene)
    {
        stopThread(100);
        currentScene = nullptr;
    }
}
