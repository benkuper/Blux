/*
  ==============================================================================

    SerialInterface.h
    Created: 26 Sep 2020 1:53:17pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SerialInterface :
    public Interface,
    public SerialDevice::SerialDeviceListener,
    public GenericControllableManager::ManagerListener
{
public:
    SerialInterface();
    ~SerialInterface();

    SerialDevice* d;
    SerialDeviceParameter* param;
    String lastOpenedPortID;

	SerialDeviceParameter* portParam;
	IntParameter* baudRate;
	SerialDevice* port;
	BoolParameter* isConnected;

    GenericControllableManager customParams;

    const Identifier sendId = "send";
    const Identifier sendBytesId = "sendBytes";

	virtual void setCurrentPort(SerialDevice* port);
	virtual bool setPortStatus(bool status);

	virtual void onContainerParameterChangedInternal(Parameter* p) override;

	virtual void sendValuesForObjectInternal(Object* o) override;

    void serialDataReceived(const var&) override;

    virtual void sendMessage(const String& message, var params = var());
    virtual void sendBytes(Array<uint8> bytes, var params = var());

    static var sendStringFromScript(const var::NativeFunctionArgs& a);
    static var sendBytesFromScript(const var::NativeFunctionArgs& a);

    static String getStringFromArgs(const var::NativeFunctionArgs& a, int offset = 0);
    static Array<uint8> getByteFromArgs(const var::NativeFunctionArgs& a, int offset = 0);

    String getTypeString() const override { return "Serial"; }
    static SerialInterface* create(var params) { return new SerialInterface(); };

    virtual void itemAdded(GenericControllableItem*) override;
    virtual void itemRemoved(GenericControllableItem*) override;


    class SerialInterfaceListener
    {
    public:
        /** Destructor. */
        virtual ~SerialInterfaceListener() {}
        virtual void customParamsChanged(SerialInterface*) {}
    };

    ListenerList<SerialInterfaceListener> customSerialListeners;
    void addSerialInterfaceListener(SerialInterfaceListener* newListener) { customSerialListeners.add(newListener); }
    void removeSerialInterfaceListener(SerialInterfaceListener* listener) { customSerialListeners.remove(listener); }

    ControllableContainer* getInterfaceParams() override { return new CustomSerialParams(this); }

    class CustomSerialParams :
        public ControllableContainer,
        public SerialInterface::SerialInterfaceListener
    {
    public:
        CustomSerialParams(SerialInterface* i);
        ~CustomSerialParams();

        SerialInterface* itf;

        void customParamsChanged(SerialInterface*);
        void rebuildArgsFromInterface();

        var getParamValues();
    };
};