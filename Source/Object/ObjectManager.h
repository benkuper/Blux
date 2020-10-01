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

    //ui
    IntParameter * gridThumbSize;

    ObjectManager();
    ~ObjectManager();

    Factory<Object> factory;

    void updateFactoryDefinitions();

    void addItemInternal(Object* o, var data) override;
    void removeItemInternal(Object* o) override;

    int getFirstAvailableObjectID();
    Object* getObjectWithID(int id, Object * excludeObject = nullptr);

    void objectIDChanged(Object * o, int previousID) override;

    void saveSceneData(var &sceneData);

    void run() override;

};