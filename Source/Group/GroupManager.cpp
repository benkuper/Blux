/*
  ==============================================================================

    GroupManager.cpp
    Created: 29 Sep 2020 9:46:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "GroupManager.h"
#include "groups/object/ObjectGroup.h"

juce_ImplementSingleton(GroupManager)

GroupManager::GroupManager() :
    BaseManager("Groups")
{
    managerFactory = &factory;

    factory.defs.add(Factory<Group>::Definition::createDef("", "Object Group", &ObjectGroup::create));
}

GroupManager::~GroupManager()
{
}
