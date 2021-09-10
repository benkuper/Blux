/*
  ==============================================================================

    ObjectManager.h
    Created: 26 Sep 2020 10:02:28am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

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
    public URL::DownloadTask::Listener
{
public:
    juce_DeclareSingleton(ObjectManager, true);

    ObjectManager();
    ~ObjectManager();

    Factory<Object> factory;

    BoolParameter* blackOut;

    //ui
    IntParameter* gridThumbSize;
    FloatParameter* defaultFlashValue;
    BoolParameter* filterActiveInScene;

    URL downloadURL;
    std::unique_ptr<URL::DownloadTask> downloadTask;
     
    void downloadObjects();
    void updateFactoryDefinitions();
    void addItemInternal(Object* o, var data) override;
    void removeItemInternal(Object* o) override;

    int getFirstAvailableObjectID(Object* excludeObject = nullptr);
    Object* getObjectWithID(int id, Object* excludeObject = nullptr);

    void objectIDChanged(Object* o, int previousID) override;

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    void run() override;

    virtual void progress(URL::DownloadTask* task, int64 downloaded, int64 total) override;
    virtual void finished(URL::DownloadTask* task, bool success) override;

};