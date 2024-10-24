/*
  ==============================================================================

    ObjectGroup.cpp
    Created: 29 Sep 2020 9:47:46pm
    Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

ObjectGroup::ObjectGroup() :
    Group(getTypeString()),
    objectsCC("Objects")
{
    objectsCC.addBaseManagerListener(this);
    objectsCC.selectItemWhenCreated = false;
    addChildControllableContainer(&objectsCC, false, 0);
}

ObjectGroup::~ObjectGroup()
{
}

void ObjectGroup::itemAdded(ObjectTarget* i)
{
    i->addObjectTargetListener(this);
    if (i->currentObject != nullptr) registerLinkedInspectable(i->currentObject);

    generateRandomIDs();
}

void ObjectGroup::itemsAdded(Array<ObjectTarget*> items)
{
    for (auto& i : items)
    {
		i->addObjectTargetListener(this);
		if (i->currentObject != nullptr) registerLinkedInspectable(i->currentObject);
	}
	generateRandomIDs();
}

void ObjectGroup::itemRemoved(ObjectTarget* i)
{
    i->removeObjectTargetListener(this);
    if(!i->objectRef.wasObjectDeleted()) unregisterLinkedInspectable(i->currentObject);
    generateRandomIDs();
}

void ObjectGroup::itemsRemoved(Array<ObjectTarget*> items)
{
    for (auto& i : items)
    {
		i->removeObjectTargetListener(this);
		if (!i->objectRef.wasObjectDeleted()) unregisterLinkedInspectable(i->currentObject);
	}
	generateRandomIDs();
}

void ObjectGroup::targetChanged(Object* newTarget, Object* previousTarget)
{
    if (previousTarget != nullptr) unregisterLinkedInspectable(previousTarget);
    if(newTarget != nullptr) registerLinkedInspectable(newTarget);
    generateRandomIDs();

}


void ObjectGroup::rebuildLinkedObjects()
{
    if (isCurrentlyLoadingData || isClearing || Engine::mainEngine->isClearing) return;
    for (auto& l : linkedInspectables)
    {
        unregisterLinkedInspectable(l);
    }
    for (auto& c : objectsCC.controllables)
    {
        if (Object* o = dynamic_cast<Object*>(((TargetParameter*)c)->targetContainer.get()))
        {
            registerLinkedInspectable(o);
        }
    }
}

void ObjectGroup::addObject(Object* o)
{
    if (o == nullptr) return;
    if (getTargetForObject(o) != nullptr) return;
    ObjectTarget* ot = objectsCC.addItem();
    ot->target->setValueFromTarget(o);
    generateRandomIDs();
}

void ObjectGroup::addObjects(Array<Object*> oList)
{
    Array<Object*> existingObjects;
    for (auto& t : objectsCC.items)
    {
        if (!t->objectRef.wasObjectDeleted()) existingObjects.add(t->currentObject);
    }

    Array<ObjectTarget*> targets;
    for (auto& o : oList)
    {
        if (existingObjects.contains(o)) continue; //already there
        ObjectTarget* ot = new ObjectTarget();
        ot->target->setValueFromTarget(o);
        targets.add(ot);
    }

    objectsCC.addItems(targets);
    generateRandomIDs();
}

ObjectTarget * ObjectGroup::getTargetForObject(Object* o)
{
    for (auto& t : objectsCC.items)
    {
        if (!t->objectRef.wasObjectDeleted() && t->currentObject == o) return t;
    }

    return nullptr;
}

bool ObjectGroup::containsObject(Object* o)
{
    return getTargetForObject(o) != nullptr;
}

int ObjectGroup::getLocalIDForObject(Object* o)
{
    int index = 0;
    for (auto& t : objectsCC.items)
    {
        if (!t->objectRef.wasObjectDeleted() && t->currentObject == o) return index;
        index++;
    }
    return -1;
}

int ObjectGroup::getRandomIDForObject(Object* o)
{
    int index = getLocalIDForObject(o);
    if (index == -1) return -1;

    return randomIDs[index];
}

int ObjectGroup::getNumObjects()
{
    return objectsCC.items.size();
}

Array<Object*> ObjectGroup::getObjects()
{
    Array<Object*> result;
    for (auto& t : objectsCC.items)
    {
        if (!t->objectRef.wasObjectDeleted() && t->currentObject != nullptr) result.add(t->currentObject);
    }
    return result;
}


ObjectTarget::ObjectTarget() :
    BaseItem("Target",false),
    currentObject(nullptr)
{
    showInspectorOnSelect = false;
    target = addTargetParameter("Target", "", ObjectManager::getInstance());
    target->maxDefaultSearchLevel = 0;
    target->targetType = TargetParameter::CONTAINER;
    target->hideInEditor = true;
    editorCanBeCollapsed = false;
}

ObjectTarget::~ObjectTarget()
{
}

void ObjectTarget::onContainerParameterChangedInternal(Parameter* p)
{
    BaseItem::onContainerParameterChangedInternal(p);

    if (p == target)
    {
        Object* prev = objectRef.wasObjectDeleted() ? nullptr : currentObject;
        currentObject = (Object *)target->targetContainer.get();
        objectRef = target->targetContainer;
        objectTargetListeners.call(&ObjectTargetListener::targetChanged, prev, currentObject);
    }
}

InspectableEditor* ObjectTarget::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
    return new ObjectTargetEditor(this, isRoot);
}
