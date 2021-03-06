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
    virtual int getLocalIDForObject(Object* o);
    virtual int getNumObjects() { return 0; }

    virtual Array<Object*> getObjects() { return Array<Object*>(); }

    void processComponentValues(Object* o, ObjectComponent* c, var& values);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);
};