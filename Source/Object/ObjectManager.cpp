/*
  ==============================================================================

    ObjectManager.cpp
    Created: 26 Sep 2020 10:02:28am
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectManager.h"

juce_ImplementSingleton(ObjectManager);

ObjectManager::ObjectManager() :
    BaseManager("Objects")
{
    managerFactory = &factory;
    factory.defs.add(Factory<Object>::Definition::createDef("", "Object", &Object::create));
}

ObjectManager::~ObjectManager()
{
}
