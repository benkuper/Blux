/*
  ==============================================================================

	DMXInterface.h
	Created: 26 Sep 2020 1:51:42pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class DMXInterface :
	public Interface,
	public DMXDevice::DMXDeviceListener
{
public:
	DMXInterface();
	~DMXInterface();

	enum DMXByteOrder { BIT8, MSB, LSB };

	EnumParameter* dmxType;

	SpinLock deviceLock;
	std::unique_ptr<DMXDevice> dmxDevice;
	BoolParameter* dmxConnected;

	BoolParameter* channelTestingMode;
	FloatParameter* channelTestingFlashValue;

	BoolParameter* sendOnChangeOnly;

	IntParameter* defaultNet;
	IntParameter* defaultSubnet;
	IntParameter* defaultUniverse;

	OwnedArray<DMXUniverse> universes;
	HashMap<int, DMXUniverse*> universeIdMap; //internally used

	void clearItem() override;

	void onContainerParameterChanged(Parameter* p) override;

	void setCurrentDMXDevice(DMXDevice* d);
	
	void dmxDeviceSetupChanged(DMXDevice*) override;

	void setDMXValue(int net, int subnet, int universe, int startChannel, Array<int> values);
	virtual void dmxDataInChanged(DMXDevice*, int net, int subnet, int universe, Array<uint8> values, const String& sourceName = "") override;

	void prepareSendValues() override;
	void sendValuesForObjectInternal(Object* o) override;
	void finishSendValues() override;

	BoolParameter* getConnectedParam() override;

	DMXUniverse* getUniverse(int net, int subnet, int universe, bool createIfNotExist = true);

	class DMXParams : public ControllableContainer
	{
	public:
		DMXParams();
		IntParameter* net;
		IntParameter* subnet;
		IntParameter* universe;
		IntParameter* startChannel;
	};

	ControllableContainer* getInterfaceParams() override { return new DMXParams(); }

	class DMXInterfaceListener
	{
	public:
		virtual ~DMXInterfaceListener() {}

		virtual void dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName = "") {}
		virtual void dmxUniverseSent(DMXUniverse* u) {}
	};

	ListenerList<DMXInterfaceListener> dmxInterfaceListeners;
	void addDMXInterfaceListener(DMXInterfaceListener* newListener) { dmxInterfaceListeners.add(newListener); }
	void removeDMXInterfaceListener(DMXInterfaceListener* listener) { dmxInterfaceListeners.remove(listener); }


	class DMXInterfaceEvent
	{
	public:
		enum Type { DATA_IN_CHANGED, UNIVERSE_SENT };

		DMXInterfaceEvent(Type t, DMXUniverse* universe, Array<uint8> values) : type(t), universe(universe), values(values) {}

		Type type;
		DMXUniverse* universe;
		Array<uint8> values;
	};

	QueuedNotifier<DMXInterfaceEvent> dmxInterfaceNotifier;
	typedef QueuedNotifier<DMXInterfaceEvent>::Listener AsyncListener;

	void addAsyncDMXInterfaceListener(AsyncListener* newListener) { dmxInterfaceNotifier.addListener(newListener); }
	void addAsyncCoalescedDMXInterfaceListener(AsyncListener* newListener) { dmxInterfaceNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncDMXInterfaceListener(AsyncListener* listener) { dmxInterfaceNotifier.removeListener(listener); }


	DECLARE_TYPE("DMX");
	static DMXInterface* create(var params) { return new DMXInterface(); };

	virtual InterfaceUI* createUI() override;
};