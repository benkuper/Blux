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
    Action(params),
    scene(nullptr),
    time(nullptr)
{
    actionType = (ActionType)(int)params.getProperty("actionType", LOAD_SCENE);
    
    
    if (actionType == LOAD_SCENE)
    {
        scene = addTargetParameter("Scene", "The scene to target", SceneManager::getInstance());
        scene->maxDefaultSearchLevel = 0;
        scene->targetType = TargetParameter::CONTAINER;
    }

    if (actionType == LOAD_SCENE || actionType == LOAD_NEXT_SCENE || actionType == LOAD_PREVIOUS_SCENE)
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
        if (Scene* s = SceneManager::getInstance()->getNextScene())
        {
            float loadTime = time->enabled ? time->floatValue() : -1;
            SceneManager::getInstance()->loadScene(s, loadTime);
        }
    }
    break;

    case LOAD_PREVIOUS_SCENE:
    {
        if (Scene* s = SceneManager::getInstance()->getPreviousScene())
        {
            float loadTime = time->enabled ? time->floatValue() : -1;
            SceneManager::getInstance()->loadScene(s, loadTime);
        }
    }
    break;
    }
}
