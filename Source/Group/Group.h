/*
  ==============================================================================

    Group.h
    Created: 29 Sep 2020 9:46:51pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Effect/EffectManager.h"
class Object;

class Group :
    public BaseItem
{
public:
    Group(String name = "Group");
    virtual ~Group();

    EffectManager effectManager;

    virtual bool containsObject(Object* o);

    void processComponentValues(Object* o, ObjectComponent* c, var& values);

    void saveSceneData(var& sceneData);
};