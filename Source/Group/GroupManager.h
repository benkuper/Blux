/*
  ==============================================================================

    GroupManager.h
    Created: 29 Sep 2020 9:46:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Group.h"

class GroupManager :
    public BaseManager<Group>
{
public:
    juce_DeclareSingleton(GroupManager, true);

    GroupManager();
    ~GroupManager();

    Factory<Group> factory;
};