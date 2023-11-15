/*
  ==============================================================================

    ObjectGroup.h
    Created: 29 Sep 2020 9:47:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;

class ObjectTarget :
    public BaseItem
{
public:
    ObjectTarget();
    ~ObjectTarget();

    Object* currentObject;
    WeakReference<Inspectable> objectRef;

    TargetParameter * target;

    void onContainerParameterChangedInternal(Parameter* p) override;

    InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;

    //Listener
    class  ObjectTargetListener
    {
    public:
        /** Destructor. */
        virtual ~ObjectTargetListener() {}
        virtual void targetChanged(Object* newTarget, Object * previousTarget) {}
    };

    ListenerList<ObjectTargetListener> objectTargetListeners;
    void addObjectTargetListener(ObjectTargetListener* newListener) { objectTargetListeners.add(newListener); }
    void removeObjectTargetListener(ObjectTargetListener* listener) { objectTargetListeners.remove(listener); }


    String getTypeString() const override { return "ObjectTarget"; }
};


class ObjectGroup :
    public Group,
    public BaseManager<ObjectTarget>::ManagerListener,
    public ObjectTarget::ObjectTargetListener
{
public:
    ObjectGroup();
    ~ObjectGroup();

    BaseManager<ObjectTarget> objectsCC;

    void itemAdded(ObjectTarget* i) override;
    void itemsAdded(Array<ObjectTarget*> items) override;
    void itemRemoved(ObjectTarget * i) override;
    void itemsRemoved(Array<ObjectTarget*> items) override;
    void targetChanged(Object * newTarget, Object * previousTarget) override;

    void rebuildLinkedObjects();

    void addObject(Object* o);
    void addObjects(Array<Object*> oList);
    ObjectTarget * getTargetForObject(Object* o);

    bool containsObject(Object* o) override;
    int getLocalIDForObject(Object* o) override;
    int getRandomIDForObject(Object* o) override;
    int getNumObjects() override;

    virtual Array<Object*> getObjects() override;


    String getTypeString() const override { return "Object Group"; }
    static ObjectGroup* create(var params) { return new ObjectGroup(); }
};