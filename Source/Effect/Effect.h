/*
  ==============================================================================

    Effect.h
    Created: 30 Sep 2020 12:38:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Filter/FilterManager.h"
class ObjectComponent;
class Object;

class Effect :
    public BaseItem
{
public:
    Effect(const String& name = "Effect", var params = var());
    ~Effect();

    FloatParameter* weight;
    FilterManager filterManager;

    void processComponentValues(Object* o, ObjectComponent* c, var& values);
    virtual var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values);

    virtual InspectableEditor* getEditor(bool isRoot) override;

    String getTypeString() const override { return "Effect"; }
};

//class ThreadedEffect :
//    public Effect,
//    public Thread
//{
//public:
//    ThreadedEffect(const String &name = "ThreadedEffect", var params = var());
//    ~ThreadedEffect();
//
//    int fps;
//
//    void onContainerParameterChangedInternal(Parameter* p);
//
//    void run() override;
//    virtual void runInternal() {}
//};