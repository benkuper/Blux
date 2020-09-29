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
    public BaseManager<Object>
{
public:
    juce_DeclareSingleton(ObjectManager, true);

    //ui
    IntParameter * gridThumbSize;

    ObjectManager();
    ~ObjectManager();

    Factory<Object> factory;


    void updateFactoryDefinitions();
};