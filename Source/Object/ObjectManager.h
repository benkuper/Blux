/*
  ==============================================================================

    ObjectManager.h
    Created: 26 Sep 2020 10:02:28am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Object.h"

class ObjectManager :
    public BaseManager<Object>,
    public Object::ObjectListener,
    public Thread
{
public:
    juce_DeclareSingleton(ObjectManager, true);

    ObjectManager();
    ~ObjectManager();


    //ui
    IntParameter* gridThumbSize;
    FloatParameter * defaultFlashValue;
    BoolParameter* blackOut;

    Factory<Object> factory;

    void updateFactoryDefinitions();

    void addItemInternal(Object* o, var data) override;
    void removeItemInternal(Object* o) override;

    int getFirstAvailableObjectID(Object * excludeObject = nullptr);
    Object* getObjectWithID(int id, Object * excludeObject = nullptr);

    void objectIDChanged(Object * o, int previousID) override;

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    void run() override;

};