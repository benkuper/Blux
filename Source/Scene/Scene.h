/*
  ==============================================================================

    Scene.h
    Created: 26 Sep 2020 1:50:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Sequence/BluxSequenceManager.h"
#include "Effect/EffectManager.h"

class Scene :
    public BaseItem
{
public:
    Scene(const String & name = "Scene");
    virtual ~Scene();

    var sceneData;
    
    Trigger* saveTrigger;
    Trigger* loadTrigger;
    FloatParameter* defaultLoadTime;

    FloatParameter* loadProgress;
    Automation interpolationCurve;
    BoolParameter* isCurrent;

    BluxSequenceManager sequenceManager;

    EffectManager effectManager;

    void saveScene();
    var getSceneData();
    void updateSceneData(); //used to resync with current objects and data that might not have been saved
    void loadScene(float loadTime = -1);

    bool isObjectActiveInScene(Object*);

    void onContainerTriggerTriggered(Trigger* t) override;

    class  SceneListener
    {
    public:
        /** Destructor. */
        virtual ~SceneListener() {}
        virtual void askForLoadScene(Scene* i, float loadTime) {}
    };

    ListenerList<SceneListener> sceneListeners;
    void addSceneListener(SceneListener* newListener) { sceneListeners.add(newListener); }
    void removeSceneListener(SceneListener* listener) { sceneListeners.remove(listener); }

    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;

    String getTypeString() const { return "Scene"; }
    static Scene* create(var params) { return new Scene(); }
};