/*
  ==============================================================================

    Scene.h
    Created: 26 Sep 2020 1:50:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Scene :
    public BaseItem,
    public Thread
{
public:
    Scene(const String & name = "Scene");
    virtual ~Scene();

    var sceneData;
    
    Trigger* saveTrigger;
    Trigger* loadTrigger;
    FloatParameter* defaultLoadTime;

    var dataAtLoad;
    var dataToLoad; //optimized list with only what has changed from state at load
    float loadTime;
    FloatParameter* loadProgress;
    Automation interpolationCurve;
    
    void saveScene();
    var getSceneData();
    void loadScene(float time = -1);

    void updateScene(); //used to resync with current objects and data that might not have been saved


    void onContainerTriggerTriggered(Trigger* t) override;

    void run() override;
    void lerpSceneParams(float weight);

    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;

    String getTypeString() const { return "Scene"; }
    static Scene* create(var params) { return new Scene(); }
};