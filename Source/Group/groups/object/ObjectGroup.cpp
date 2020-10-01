/*
  ==============================================================================

    ObjectGroup.cpp
    Created: 29 Sep 2020 9:47:46pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectGroup.h"
#include "Object/ObjectManager.h"

ObjectGroup::ObjectGroup() :
    Group(getTypeString()),
    objectsCC("Objects")
{
    objectsCC.userCanAddControllables = true;
    objectsCC.customUserCreateControllableFunc = &ObjectGroup::createObjectTarget;
    addChildControllableContainer(&objectsCC, false, 0);
}

ObjectGroup::~ObjectGroup()
{
}

TargetParameter * ObjectGroup::createObjectTarget(ControllableContainer* cc)
{
    TargetParameter* tp = cc->addTargetParameter("Object", "Object to be part of this group", ObjectManager::getInstance());
    tp->targetType = TargetParameter::CONTAINER;
    tp->maxDefaultSearchLevel = 0;
    tp->saveValueOnly = false;
    tp->isRemovableByUser = true;
    return tp;
}

void ObjectGroup::childStructureChanged(ControllableContainer* cc)
{
    Group::childStructureChanged(cc);
    if (isCurrentlyLoadingData) return;
    if (cc == &objectsCC) rebuildLinkedObjects();
}

void ObjectGroup::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
    Group::onControllableFeedbackUpdateInternal(cc, c);
    if (isCurrentlyLoadingData) return;

    if (cc == &objectsCC)
    {
        rebuildLinkedObjects();
    }
}

void ObjectGroup::rebuildLinkedObjects()
{
    if (isCurrentlyLoadingData) return;
    for (auto& l : linkedInspectables) unregisterLinkedInspectable(l);
    for (auto& c : objectsCC.controllables)
    {
        if (Object* o = dynamic_cast<Object*>(((TargetParameter*)c)->targetContainer.get())) registerLinkedInspectable(o);
    }
}

void ObjectGroup::addObject(Object* o)
{
    if (o == nullptr) return;
    if (getTargetParamForObject(o) != nullptr) return;
    TargetParameter* tp = ObjectGroup::createObjectTarget(&objectsCC);
    tp->setValueFromTarget(o);
}

TargetParameter* ObjectGroup::getTargetParamForObject(Object* o)
{
    for (auto& c : objectsCC.controllables)
    {
        TargetParameter* tp = (TargetParameter*)c;
        if (tp->targetContainer == o) return tp;
    }

    return nullptr;
}

bool ObjectGroup::containsObject(Object* o)
{
    return getTargetParamForObject(o) != nullptr;
}

var ObjectGroup::getJSONData()
{
    var data = Group::getJSONData();
    var oData;
    for (auto& tp : objectsCC.controllables)
    {
        if (Object* o = dynamic_cast<Object*>(((TargetParameter*)tp)->targetContainer.get())) oData.append(o->shortName);
    }
    data.getDynamicObject()->setProperty("objects", oData);

    return data;
}

void ObjectGroup::loadJSONDataInternal(var data)
{
    Group::loadJSONDataInternal(data);
    var oData = data.getProperty("objects", var());
    for (int i = 0; i < oData.size(); i++) addObject(ObjectManager::getInstance()->getItemWithName(oData[i], false));
}

void ObjectGroup::afterLoadJSONDataInternal()
{
    rebuildLinkedObjects();
}
