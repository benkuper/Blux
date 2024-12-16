/*
  ==============================================================================

    Scene.h
    Created: 26 Sep 2020 1:50:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Scene :
    public BaseItem,
    public EffectManager::ManagerListener,
    public SequenceManager::ManagerListener
{
public:
    Scene(const String & name = "Scene");
    virtual ~Scene();

    var sceneData;
    
    Trigger* saveTrigger;
    Trigger* loadTrigger;
    Trigger* directLoadTrigger;
    FloatParameter* defaultLoadTime;

    FloatParameter* loadProgress;
    Automation interpolationCurve;
    BoolParameter* isCurrent;

    std::unique_ptr<BluxSequenceManager> sequenceManager;
    std::unique_ptr<EffectManager> effectManager;

    ActionManager loadActions;
    ActionManager unloadActions;

    void saveScene();
    var getSceneData();
    void updateSceneData(); //used to resync with current objects and data that might not have been saved
    void loadScene(float loadTime = -1);

    bool isObjectActiveInScene(Object*);

    void onContainerTriggerTriggered(Trigger* t) override;
    void onContainerParameterChangedInternal(Parameter* p) override;

    void itemAdded(Sequence* s) override;
    void itemsAdded(Array<Sequence*> items) override;

    void resetEffectTimes();

    void itemAdded(Effect* e) override;
    void itemsAdded(Array<Effect*> items) override;

    var getJSONData(bool includeNonOverriden = false) override;
    void loadJSONDataItemInternal(var data) override;

    String getTypeString() const override { return "Scene"; }
    static Scene* create(var params) { return new Scene(); }

    ListenerList<SceneListener> sceneListeners;
    void addSceneListener(SceneListener* newListener) { sceneListeners.add(newListener); }
    void removeSceneListener(SceneListener* listener) { sceneListeners.remove(listener); }
};
