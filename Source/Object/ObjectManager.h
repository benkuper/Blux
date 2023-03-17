/*
  ==============================================================================

    ObjectManager.h
    Created: 26 Sep 2020 10:02:28am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ObjectManagerCustomParams;

class SubObjectManager :
    public BaseManager<Object>
{
public:
    SubObjectManager();
    ~SubObjectManager();
};


class ObjectManager :
    public BaseManager<Object>,
    public Object::ObjectListener,
    public Thread,
    public URL::DownloadTask::Listener,
    public GenericControllableManager::ManagerListener
{
public:
    juce_DeclareSingleton(ObjectManager, true);

    ObjectManager();
    ~ObjectManager();

    virtual void clear() override;

    Factory<Object> factory;

    BoolParameter* blackOut;
    IntParameter* updateRate;

    //ui
    IntParameter* gridThumbSize;
    FloatParameter* defaultFlashValue;
    BoolParameter* filterActiveInScene;
    BoolParameter* lockUI;

    URL downloadURL;
    std::unique_ptr<URL::DownloadTask> downloadTask;

    GenericControllableManager customParams;

    virtual void itemAdded(GenericControllableItem*) override;
    virtual void itemRemoved(GenericControllableItem*) override;

    void downloadObjects();
    void updateFactoryDefinitions();
    void addItemInternal(Object* o, var data) override;
    void removeItemInternal(Object* o) override;

    int getFirstAvailableObjectID(Object* excludeObject = nullptr);
    Object* getObjectWithID(int id, Object* excludeObject = nullptr);

    void objectIDChanged(Object* o, int previousID) override;

    void onContainerParameterChanged(Parameter* p) override;

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    virtual var getVizData();


    void run() override;

    virtual void progress(URL::DownloadTask* task, int64 downloaded, int64 total) override;
    virtual void finished(URL::DownloadTask* task, bool success) override;

    var getJSONData() override;
    void loadJSONDataManagerInternal(var data) override;
    void afterLoadJSONDataInternal() override;

    //Listener
    class ObjectManagerListener
    {
    public:
        /** Destructor. */
        virtual ~ObjectManagerListener() {}
        virtual void updateStart() {}
        virtual void updateFinish() {}
        virtual void customParamsChanged(ObjectManager*) {}
    };

    ListenerList<ObjectManagerListener> objectManagerListeners;
    void addObjectManagerListener(ObjectManagerListener* newListener) { objectManagerListeners.add(newListener); }
    void removeObjectManagerListener(ObjectManagerListener* listener) { objectManagerListeners.remove(listener); }

    ObjectManagerCustomParams* getCustomParams();
};

class ObjectManagerCustomParams :
    public ControllableContainer,
    public ObjectManager::ObjectManagerListener
{
public:
    ObjectManagerCustomParams(ObjectManager* om);
    ~ObjectManagerCustomParams();
    
    ObjectManager* om;

    void customParamsChanged(ObjectManager*);
    void rebuildCustomParams();

    var getParamValueFor(WeakReference<Parameter> p);
    var getParamValueForName(const String& name);
    var getParamValues();

    Parameter * getActiveParamFor(WeakReference<Parameter> p);
    Parameter * getActiveCustomParamForName(const String& name);
};
