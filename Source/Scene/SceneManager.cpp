#include "SceneManager.h"
/*
  ==============================================================================

    SceneManager.cpp
    Created: 26 Sep 2020 1:49:54pm
    Author:  bkupe

  ==============================================================================
*/
juce_ImplementSingleton(SceneManager)

SceneManager::SceneManager() :
    BaseManager("Scenes"),
    Thread("Scene Load"),
    previousScene(nullptr),
    currentScene(nullptr),
    sceneManagerNotifier(5)
{
    managerFactory = &factory;
    factory.defs.add(Factory<Scene>::Definition::createDef("", "Scene", &Scene::create));

    loadNextSceneTrigger = addTrigger("Load Next Scene", "Load the next scene. If no scene is loaded, will load first one");
    loadPreviousSceneTrigger = addTrigger("Load Previous Scene", "Load the previous scene. If no scene is loaded, this will do nothing.");

    autoPreview = addBoolParameter("Auto Preview", "If checked, this will force preview when hovering a scene",  false);

    lockUI = addBoolParameter("Lock UI", "If checked, all UI will be locked", false);
}

SceneManager::~SceneManager()
{
    threadShouldExit();
}


void SceneManager::addItemInternal(Scene* s, var data)
{
    s->addSceneListener(this);
    s->effectManager->setForceDisabled(true);
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

Scene* SceneManager::getNextScene()
{
    int index = items.indexOf(currentScene) + 1;
    return index < items.size() ? items[index] : nullptr;;
}

Scene* SceneManager::getPreviousScene()
{
    int index = items.indexOf(currentScene) - 1;
    return index >= 0 ? items[index] : nullptr;
}

void SceneManager::run()
{
    String oName = ObjectManager::getInstance()->shortName;
    String gName = GroupManager::getInstance()->shortName;
    String eName = GlobalEffectManager::getInstance()->shortName;

    currentScene->loadProgress->setValue(0);
    currentScene->effectManager->setForceDisabled(false);
    currentScene->resetEffectTimes();
    for (auto& s : currentScene->sequenceManager->items)
    {
        if (s->startAtLoad->boolValue())
        {
            s->stopTrigger->trigger();
            s->playTrigger->trigger();
        }
    }
     
    sceneManagerNotifier.addMessage(new SceneManagerEvent(SceneManagerEvent::SCENE_LOAD_START));

    
    var dataAtLoad = currentScene->getSceneData();
    double timeAtLoad = Time::getMillisecondCounter() / 1000.0;
    if (loadTime > 0)
    {
        while (!threadShouldExit() && currentScene->loadProgress->floatValue() < 1)
        {
            if (Engine::mainEngine->isClearing) return;

            double curTime = Time::getMillisecondCounter() / 1000.0;
            double progress = (curTime - timeAtLoad) / loadTime;
            currentScene->loadProgress->setValue(progress);

            float weight = currentScene->interpolationCurve.getValueAtPosition(progress);

            ObjectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(oName, var()), currentScene->sceneData.getProperty(oName, var()), weight);
            GroupManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(gName, var()), currentScene->sceneData.getProperty(gName, var()), weight);
            GlobalEffectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(eName, var()), currentScene->sceneData.getProperty(eName, var()), weight);

            sleep(30);
        }
    }

    if (Engine::mainEngine->isClearing) return;

    
    if (currentScene->loadProgress->floatValue() == 1 || loadTime == 0)
    {
        ObjectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(oName, var()), currentScene->sceneData.getProperty(oName, var()), 1);
        GroupManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(gName, var()), currentScene->sceneData.getProperty(gName, var()), 1);
        GlobalEffectManager::getInstance()->lerpFromSceneData(dataAtLoad.getProperty(eName, var()), currentScene->sceneData.getProperty(eName, var()), 1);
        currentScene->isCurrent->setValue(true);
    }

    currentScene->loadProgress->setValue(0);

    if (previousScene != nullptr && previousScene != currentScene)
    {
        for (auto& s : previousScene->sequenceManager->items)
        {
            s->stopTrigger->trigger();
        }

        previousScene->effectManager->setForceDisabled(true);
    }
   
    sceneManagerNotifier.addMessage(new SceneManagerEvent(SceneManagerEvent::SCENE_LOAD_END));
}

void SceneManager::askForLoadScene(Scene* s, float loadTime)
{
    loadScene(s, loadTime);
}

Array<ChainVizTarget *> SceneManager::getChainVizTargetsForObject(Object* o)
{
    Array<ChainVizTarget *> result;
    if (currentScene == nullptr) return result;

    result.addArray(currentScene->sequenceManager->getChainVizTargetsForObject(o));
    result.addArray(currentScene->effectManager->getChainVizTargetsForObject(o));
    return result;
}

void SceneManager::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    if (currentScene == nullptr) return;

    float progressWeight = currentScene->isCurrent->boolValue() ? 1 : currentScene->loadProgress->floatValue();
    if (previousScene != nullptr && progressWeight < 1)
    {
        previousScene->sequenceManager->processComponentValues(o, c, values, 1 - currentScene->loadProgress->floatValue());
        previousScene->effectManager->processComponentValues(o, c, values, 1 - currentScene->loadProgress->floatValue());
    }

    currentScene->sequenceManager->processComponentValues(o, c, values, progressWeight);
    currentScene->effectManager->processComponentValues(o, c, values,  progressWeight);
}

void SceneManager::onContainerTriggerTriggered(Trigger* t)
{
    if (t == loadNextSceneTrigger)
    {
        if (Scene* s = getNextScene()) s->loadTrigger->trigger();
    }
    else if (t == loadPreviousSceneTrigger)
    {
        if (Scene* s = getPreviousScene()) s->loadTrigger->trigger();
    }
}

void SceneManager::onContainerParameterChanged(Parameter* p)
{
    if (p == lockUI) for (auto& i : items) i->isUILocked->setValue(lockUI->boolValue());
}


void SceneManager::inspectableDestroyed(Inspectable* i)
{
    if (i == currentScene)
    {
        stopThread(100);
        currentScene = nullptr;
    }
}
