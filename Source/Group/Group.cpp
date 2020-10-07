/*
  ==============================================================================

    Group.cpp
    Created: 29 Sep 2020 9:46:51pm
    Author:  bkupe

  ==============================================================================
*/

#include "Group.h"
#include "Object/Object.h"

Group::Group(String name) :
    BaseItem(name)
{
    saveAndLoadRecursiveData = true;
 
    addChildControllableContainer(&effectManager);
}

Group::~Group()
{

}

bool Group::containsObject(Object* o)
{
    return false;
}

int Group::getLocalIDForObject(Object* o)
{
    return -1;
}

void Group::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    effectManager.processComponentValues(o, c, values);
}

void Group::saveSceneData(var& sceneData)
{
    sceneData.getDynamicObject()->setProperty(enabled->getControlAddress(), enabled->boolValue());
    effectManager.saveSceneData(sceneData);
}
