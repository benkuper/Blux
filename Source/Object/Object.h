/*
  ==============================================================================

    Object.h
    Created: 26 Sep 2020 10:02:32am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Component/ComponentManager.h"
#include "Effect/EffectManager.h"

class Object :
    public BaseItem
{
public:
    Object(var params = var());
    virtual ~Object();

    String objectType;
    var objectData;

    TargetParameter* targetInterface;
    std::unique_ptr<ControllableContainer> interfaceParameters;
    var interfaceGhostData;

    ComponentManager componentManager;
    EffectManager effectManager;

    IntParameter* globalID;
    int previousID;
    
    BoolParameter* excludeFromScenes;

    Point3DParameter* stagePosition;

    //ui
    File customThumbnailPath;
    Parameter* slideManipParameter;
    float slideManipValueRef;


    void rebuildInterfaceParams();

    template<class T>
    T* getComponent();

    void onContainerParameterChangedInternal(Parameter* p) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

    void checkAndComputeComponentValuesIfNeeded();
    void computeComponentValues(ObjectComponent* c);

    void saveSceneData(var &sceneData);

    //Listener
    class  ObjectListener
    {
    public:
        /** Destructor. */
        virtual ~ObjectListener() {}
        virtual void objectIDChanged(Object * o, int previousID) {}
    };

    ListenerList<ObjectListener> objectListeners;
    void addObjectListener(ObjectListener* newListener) { objectListeners.add(newListener); }
    void removeObjectListener(ObjectListener* listener) { objectListeners.remove(listener); }


    String getTypeString() const override { return objectType; }
    static Object* create(var params) { return new Object(params); }
};

template<class T>
T* Object::getComponent()
{
    for (auto& c : componentManager.items)
    {
        if (T* result = dynamic_cast<T*>(c)) return result;
    }
    return nullptr;
}
