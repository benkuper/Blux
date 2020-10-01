/*
  ==============================================================================

    GroupManager.h
    Created: 29 Sep 2020 9:46:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Group.h"
class Object;
class ObjectComponent;

class GroupManager :
    public BaseManager<Group>
{
public:
    juce_DeclareSingleton(GroupManager, true);

    GroupManager();
    ~GroupManager();

    Factory<Group> factory;

    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values);
};