/*
  ==============================================================================

    SceneAction.cpp
    Created: 8 Oct 2020 3:32:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "SceneAction.h"
#include "Scene/SceneManager.h"

SceneAction::SceneAction(var params) :
    TimeTrigger("Scene Action"),
    scene(nullptr),
    time(nullptr)
{
    actionType = (ActionType)(int)params.getProperty("type", LOAD_SCENE);
    
    
    if (actionType == LOAD_SCENE)
    {
        scene = addTargetParameter("Scene", "The scene to target", SceneManager::getInstance());
        scene->maxDefaultSearchLevel = 0;
        scene->targetType = TargetParameter::CONTAINER;
    }

    if (actionType == LOAD_SCENE || actionType == LOAD_NEXT_SCENE)
    {
        time = addFloatParameter("Load Time", "If enabled, this will override the default load time of this scene", 1, 0, INT32_MAX, false);
        time->canBeDisabledByUser = true;
        time->defaultUI = FloatParameter::TIME;
    }
}

SceneAction::~SceneAction()
{
}

void SceneAction::triggerInternal()
{
    switch (actionType)
    {
    case LOAD_SCENE:
    {
        if (Scene* s = dynamic_cast<Scene*>(scene->targetContainer.get()))
        {
            float loadTime = time->enabled ? time->floatValue() : -1;
            SceneManager::getInstance()->loadScene(s, loadTime);
        }
    }
    break;

    case LOAD_NEXT_SCENE:
    {
        int index = SceneManager::getInstance()->items.indexOf(SceneManager::getInstance()->currentScene) + 1;
        if (index < SceneManager::getInstance()->items.size())
        {
            Scene* s = SceneManager::getInstance()->items[index];
            float loadTime = time->enabled ? time->floatValue() : -1;
            SceneManager::getInstance()->loadScene(s, loadTime);
        }
    }
    break;
    }
}
