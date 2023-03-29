/*
  ==============================================================================

    Interface.h
    Created: 26 Sep 2020 1:50:32pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class ObjectComponent;
class InterfaceUI;

class Interface :
    public BaseItem
{
public:
    Interface(String name = "Interface", bool canHaveScript = false);
    virtual ~Interface();

    BoolParameter* logIncomingData;
    BoolParameter* logOutgoingData;

    //Do not include in hierarchy to avoid going crazy on those listeners
    std::unique_ptr<Trigger> inActivityTrigger;
    std::unique_ptr<Trigger> outActivityTrigger;

    virtual void prepareSendValues() {}
    virtual void sendValuesForObject(Object* o);
    virtual void sendValuesForObjectInternal(Object* o) {}
    virtual void finishSendValues() {}

    virtual ControllableContainer* getInterfaceParams() { return new ControllableContainer("Interface parameters"); }

    virtual InterfaceUI* createUI();
};