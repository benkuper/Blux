/*
  ==============================================================================

    Object.h
    Created: 26 Sep 2020 10:02:32am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Component/ComponentManager.h"

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

    //ui
    File customThumbnailPath;
    Parameter* slideManipParameter;
    float slideManipValueRef;

    void rebuildInterfaceParams();

    template<class T>
    T* getComponent();

    void onContainerParameterChangedInternal(Parameter* p) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

    void computeAndSendValue(ObjectComponent * c, Parameter* p);

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
