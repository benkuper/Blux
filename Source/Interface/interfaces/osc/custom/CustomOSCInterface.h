/*
  ==============================================================================

    CustomOSCInterface.h
    Created: 18 Nov 2021 3:03:41pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CustomOSCInterface :
    public OSCInterface,
    public GenericControllableManager::ManagerListener
{
public:
    CustomOSCInterface();
    ~CustomOSCInterface();

    virtual void itemAdded(GenericControllableItem*) override;
    virtual void itemsAdded(Array<GenericControllableItem*>) override;
    virtual void itemRemoved(GenericControllableItem*) override;
    virtual void itemsRemoved(Array<GenericControllableItem*>) override;

    virtual void sendValuesForObjectInternal(Object* o) override;

    GenericControllableManager customParams;

    //Listener
    class CustomOSCInterfaceListener
    {
    public:
        /** Destructor. */
        virtual ~CustomOSCInterfaceListener() {}
        virtual void customParamsChanged(CustomOSCInterface *) {}
    };

    ListenerList<CustomOSCInterfaceListener> customOSCListeners;
    void addCustomOSCInterfaceListener(CustomOSCInterfaceListener* newListener) { customOSCListeners.add(newListener); }
    void removeCustomOSCInterfaceListener(CustomOSCInterfaceListener* listener) { customOSCListeners.remove(listener); }

    ControllableContainer* getInterfaceParams() override { return new CustomOSCParams(this); }

    class CustomOSCParams :
        public ControllableContainer,
        public CustomOSCInterface::CustomOSCInterfaceListener
    {
    public:
        CustomOSCParams(CustomOSCInterface* i);
        ~CustomOSCParams();

        CustomOSCInterface* itf;

        void customParamsChanged(CustomOSCInterface*);
        void rebuildArgsFromInterface();

        var getParamValues();
    };

    String getTypeString() const override { return "OSC"; }
    static CustomOSCInterface* create(var params) { return new CustomOSCInterface(); };
};