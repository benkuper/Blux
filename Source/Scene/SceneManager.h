/*
  ==============================================================================

    SceneManager.h
    Created: 26 Sep 2020 1:49:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Scene.h"
class Object;
class ObjectComponent;

class SceneManager :
    public BaseManager<Scene>,
    public Inspectable::InspectableListener,
    public Scene::SceneListener,
    public Thread
{
public:
    juce_DeclareSingleton(SceneManager, true);

    SceneManager();
    ~SceneManager();

    Factory<Scene> factory;

    var dataAtLoad;
    var dataToLoad; //optimized list with only what has changed from state at load
    float loadTime;

    Scene* previousScene;
    Scene* currentScene;

    void addItemInternal(Scene* s, var data) override;
    void removeItemInternal(Scene* s) override;

    void loadScene(Scene* s, float loadTime = -1);

    void run() override;
    void lerpSceneParams(float weight);

    void askForLoadScene(Scene* s, float loadTime) override;

    void inspectableDestroyed(Inspectable* i) override;

    void processComponentValues(Object* o, ObjectComponent* c, var& values);

};