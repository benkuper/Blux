/*
  ==============================================================================

    ObjectGroup.h
    Created: 29 Sep 2020 9:47:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Group.h"

class Object;

class ObjectGroup :
    public Group
{
public:
    ObjectGroup();
    ~ObjectGroup();

    ControllableContainer objectsCC;

    void childStructureChanged(ControllableContainer* cc) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
    void rebuildLinkedObjects();

    void addObject(Object* o);
    TargetParameter* getTargetParamForObject(Object* o);

    bool containsObject(Object* o) override;


    static TargetParameter* createObjectTarget(ControllableContainer* cc);

    var getJSONData() override;
    void loadJSONDataInternal(var data) override;
    void afterLoadJSONDataInternal() override;

    String getTypeString() const override { return "Object Group"; }
    static ObjectGroup* create(var params) { return new ObjectGroup(); }
};