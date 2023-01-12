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
	public DMXDevice::DMXDeviceListener,
	public Thread
{
public:
	DMXInterface();
	~DMXInterface();

	enum DMXByteOrder { BIT8, MSB, LSB };

	EnumParameter* dmxType;

	SpinLock sendLock;
	std::unique_ptr<DMXDevice> dmxDevice;
	BoolParameter* dmxConnected;

	BoolParameter* channelTestingMode;
	FloatParameter* channelTestingFlashValue;

	IntParameter* sendRate;
	BoolParameter* sendOnChangeOnly;
	BoolParameter* useIntensityForColor;

	IntParameter* defaultNet;
	IntParameter* defaultSubnet;
	IntParameter* defaultUniverse;


	OwnedArray<DMXUniverse> universes;
	HashMap<int, DMXUniverse*> universeIdMap; //internally used


	void clearItem() override;

	void onContainerParameterChanged(Parameter* p) override;

	void setCurrentDMXDevice(DMXDevice* d);

	//void setDMXValue(int net, int subnet, int universe, int startChannel, Array<int> values);
	//void sendDMXValues(int net, int subnet, int universe, int startChannel, Array<int> values);
	//void sendDMXValue(int net, int subnet, int universe, int channel, int value);
	//void send16BitDMXValue(int net, int subnet, int universe, int startChannel, int value, DMXByteOrder byteOrder);
	//void send16BitDMXValues(int net, int subnet, int universe, int startChannel, Array<int> values, DMXByteOrder byteOrder);

	void dmxDeviceConnected() override;
	void dmxDeviceDisconnected() override;

	void dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName = "") override;
	void sendValuesForObjectInternal(Object* o) override;

	DMXUniverse* getUniverse(int net, int subnet, int universe, bool createIfNotExist = true);

	void run() override;

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
	};

	ListenerList<DMXInterfaceListener> dmxInterfaceListeners;
	void addDMXInterfaceListener(DMXInterfaceListener* newListener) { dmxInterfaceListeners.add(newListener); }
	void removeDMXInterfaceListener(DMXInterfaceListener* listener) { dmxInterfaceListeners.remove(listener); }


	DECLARE_TYPE("DMX");
	static DMXInterface* create(var params) { return new DMXInterface(); };

	virtual InterfaceUI* createUI() override;
};