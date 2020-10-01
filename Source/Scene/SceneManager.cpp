/*
  ==============================================================================

    SceneManager.cpp
    Created: 26 Sep 2020 1:49:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "SceneManager.h"

juce_ImplementSingleton(SceneManager)

SceneManager::SceneManager() :
    BaseManager("Scenes")
{
    managerFactory = &factory;
    factory.defs.add(Factory<Scene>::Definition::createDef("", "Scene", &Scene::create));
}

SceneManager::~SceneManager()
{
}

void SceneManager::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    //to implement
}
