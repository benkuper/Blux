/*
  ==============================================================================

	SceneManager.cpp
	Created: 26 Sep 2020 1:49:54pm
	Author:  bkupe

  ==============================================================================
*/

#include "Scene/SceneIncludes.h"

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

	addLoadToUndo = addBoolParameter("Undoable Load", "If checked, this will add the load of a scene to the Undo Manager", true);
	forceLoadTime = addFloatParameter("Force Load Time", "If enabled, this will always load with this timing", .5f, 0);
	forceLoadTime->defaultUI = FloatParameter::TIME;
	forceLoadTime->setEnabled(false);
	forceLoadTime->canBeDisabledByUser = true;

	loadNextSceneTrigger = addTrigger("Load Next Scene", "Load the next scene. If no scene is loaded, will load first one");
	loadPreviousSceneTrigger = addTrigger("Load Previous Scene", "Load the previous scene. If no scene is loaded, this will do nothing.");

	previewMode = addEnumParameter("Preview Mode", "The mode to use to preview values from a different scene");
	previewMode->addOption("None", NONE)->addOption("Next Scene", NEXT)->addOption("Hover", HOVER)->addOption("Selected", SELECTED);

	lockUI = addBoolParameter("Lock UI", "If checked, all UI will be locked", false);

	OSCRemoteControl::getInstance()->addRemoteControlListener(this);
}

SceneManager::~SceneManager()
{
	stopThread(1000);
	if (OSCRemoteControl::getInstanceWithoutCreating() != nullptr) OSCRemoteControl::getInstance()->removeRemoteControlListener(this);

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

void SceneManager::loadScene(Scene* s, float time, bool setUndoIfNeeded)
{
	if (s == nullptr) return;

	stopThread(1000);

	if (forceLoadTime->enabled) time = forceLoadTime->floatValue();

	if (setUndoIfNeeded && addLoadToUndo->boolValue() && currentScene != nullptr && s != nullptr)
	{
		UndoMaster::getInstance()->performAction("Load Scene " + currentScene->niceName, new SceneLoadAction(currentScene, loadTime, s, time));
		return;
	}


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


	var dataAtLoad = currentScene->getSceneData().clone();
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

Array<ChainVizTarget*> SceneManager::getChainVizTargetsForObjectAndComponent(Object* o, ComponentType c)
{
	Array<ChainVizTarget*> result;
	if (currentScene == nullptr) return result;

	result.addArray(currentScene->sequenceManager->getChainVizTargetsForObjectAndComponent(o, c));
	result.addArray(currentScene->effectManager->getChainVizTargetsForObjectAndComponent(o, c));
	return result;
}

void SceneManager::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values)
{
	if (currentScene == nullptr) return;

	float progressWeight = currentScene->isCurrent->boolValue() ? 1 : currentScene->loadProgress->floatValue();

	if (previousScene != nullptr && progressWeight < 1)
	{
		HashMap<Parameter*, var> prevSceneValues;
		//HashMap<Parameter*, var> newSceneValues(values.size());

		HashMap<Parameter*, var>::Iterator copyIt(values);
		while (copyIt.next()) prevSceneValues.set(copyIt.getKey(), copyIt.getValue().clone());
		;
		previousScene->sequenceManager->processComponent(o, c, prevSceneValues);
		previousScene->effectManager->processComponent(o, c, prevSceneValues);

		currentScene->sequenceManager->processComponent(o, c, values);
		currentScene->effectManager->processComponent(o, c, values);

		//float invertProgress = 1 - progressWeight;

		Array<Parameter*> processedParams;

		HashMap<Parameter*, var>::Iterator it(values);

		while (it.next())
		{
			Parameter* cp = it.getKey();
			var prevVal = prevSceneValues[cp];
			var newVal = it.getValue().clone();
			if (prevVal != newVal)
			{
				jassert(prevVal.size() == newVal.size());

				var val;
				if (newVal.isArray())
				{
					for (int i = 0; i < newVal.size(); i++)
					{
						if (newVal[i].isArray())
						{
							var vv;
							for (int j = 0; j < newVal[i].size(); j++) vv.append(jmap(progressWeight, (float)prevVal[i][j], (float)newVal[i][j]));
							val.append(vv);
						}
						else
						{
							val.append(jmap(progressWeight, (float)prevVal[i], (float)newVal[i]));
						}
					}
				}
				else
				{
					val = jmap(progressWeight, (float)prevVal, (float)newVal);
				}

				values.set(cp, val);
			}
		}
	}
	else
	{
		currentScene->sequenceManager->processComponent(o, c, values);
		currentScene->effectManager->processComponent(o, c, values);
	}


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

void SceneManager::processMessage(const OSCMessage& m)
{
	StringArray addSplit;
	addSplit.addTokens(m.getAddressPattern().toString(), "/", "\"");

	if (addSplit.size() < 3 || addSplit[1] != "scenes") return;

	String cmd = addSplit[2];
	if (cmd == "loadScene")
	{
		if (m.size() > 0)
		{
			String sceneName = OSCHelpers::getStringArg(m[0]);
			if (Scene* s = getItemWithName(sceneName, true))
			{
				loadScene(s, m.size() > 1 ? OSCHelpers::getFloatArg(m[1]) : -1);
			}
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

SceneManager::SceneLoadAction::SceneLoadAction(Scene* prevScene, float prevTime, Scene* s, float time) :
	prevScene(prevScene), prevSceneRef(prevScene), prevTime(prevTime), scene(s), sceneRef(s), time(time)
{
}



bool SceneManager::SceneLoadAction::perform()
{
	if (sceneRef == nullptr || sceneRef.wasObjectDeleted()) return false;

	SceneManager::getInstance()->loadScene(scene, time, false);
	return true;
}

bool SceneManager::SceneLoadAction::undo()
{
	if (prevSceneRef == nullptr || prevSceneRef.wasObjectDeleted()) return false;
	SceneManager::getInstance()->loadScene(prevScene, prevTime, false);
	return true;
}
