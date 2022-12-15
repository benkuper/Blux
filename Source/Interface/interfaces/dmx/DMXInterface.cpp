/*
  ==============================================================================

	DMXInterface.cpp
	Created: 26 Sep 2020 1:51:42pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface/InterfaceIncludes.h"
#include "Object/ObjectIncludes.h"

DMXInterface::DMXInterface() :
	Interface(getTypeString()),
	Thread("DMX Interface")
{
	dmxType = addEnumParameter("DMX Type", "Choose the type of dmx interface you want to connect");

	dmxType->addOption("Open DMX", DMXDevice::OPENDMX)->addOption("Enttec DMX Pro", DMXDevice::ENTTEC_DMXPRO)->addOption("Enttec DMX MkII", DMXDevice::ENTTEC_MK2)->addOption("Art-Net", DMXDevice::ARTNET);
	dmxType->setValueWithKey("Open DMX");

	dmxConnected = addBoolParameter("Connected", "DMX is connected ?", false);
	dmxConnected->isControllableFeedbackOnly = true;
	dmxConnected->isSavable = false;

	defaultNet = addIntParameter("Net", "If appliccable the net for this universe", 0, 0, 15, false);
	defaultSubnet = addIntParameter("Subnet", "If applicable the subnet for this universe", 0, 0, 15, false);
	defaultUniverse = addIntParameter("Universe", "The universe", 0, 0, false);

	sendRate = addIntParameter("Send Rate", "The rate at which to send data.", 40, 1, 200);
	sendOnChangeOnly = addBoolParameter("Send On Change Only", "Only send a universe if one of its channels has changed", true);



	channelTestingMode = addBoolParameter("Channel Testing Mode", "Is testing with the Channel view ?", false);
	channelTestingMode->hideInEditor = true;

	channelTestingFlashValue = addFloatParameter("Channel Testing Flash Value", "Flash value of channel testing", 1, 0, 1);
	channelTestingFlashValue->hideInEditor = true;

	setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));

	//for (int i = 0; i < DMX_MAX_UNIVERSES;i++)
	//{
		//DMXUniverse * u = getUniverse(i >> 8 & 0xf, i >> 4 & 0xf, i & 0x7f);
		//DBG("Create " << u->toString());
	//}
}

DMXInterface::~DMXInterface()
{
}

void DMXInterface::clearItem()
{
	BaseItem::clearItem();
	setCurrentDMXDevice(nullptr);
}

void DMXInterface::onContainerParameterChanged(Parameter* p)
{
	Interface::onContainerParameterChanged(p);
	if (p == enabled)
	{
		if (dmxDevice != nullptr) dmxDevice->enabled = enabled->boolValue();
		if (enabled->boolValue() && dmxDevice != nullptr) startThread();
		else stopThread(1000);

	}
	else if (p == dmxType)
	{
		setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
	}
}

void DMXInterface::setCurrentDMXDevice(DMXDevice* d)
{
	if (dmxDevice.get() == d) return;

	stopThread(1000);

	GenericScopedLock lock(deviceLock);

	if (dmxDevice != nullptr)
	{
		dmxDevice->removeDMXDeviceListener(this);
		dmxDevice->clearDevice();
		removeChildControllableContainer(dmxDevice.get());
	}

	dmxDevice.reset(d);

	dmxConnected->hideInEditor = dmxDevice == nullptr || dmxDevice->type == DMXDevice::ARTNET;
	dmxConnected->setValue(false);

	if (dmxDevice != nullptr)
	{
		dmxDevice->enabled = enabled->boolValue();
		dmxDevice->addDMXDeviceListener(this);
		addChildControllableContainer(dmxDevice.get());
		dmxConnected->setValue(true);
	}

	if (enabled->boolValue() && dmxDevice != nullptr) startThread();
}

//void DMXInterface::sendDMXValue(int channel, int value)
//{
//	if (!enabled->boolValue() || dmxDevice == nullptr) return;
//	if (logOutgoingData->boolValue()) NLOG(niceName, "Send DMX : " + String(channel) + " > " + String(value));
//	//dmxDevice->sendDMXValue(0, 0, 0, channel, value);
//}
//

//void DMXInterface::sendDMXValues(int net, int subnet, int universe, Array<int> values)
//{
//	if (!enabled->boolValue() || dmxDevice == nullptr) return;
//	if (logOutgoingData->boolValue())
//	{
//		String s = "Send DMX : " + String(startChannel) + ", " + String(values.size()) + " values";
//		int ch = startChannel;
//		for (auto& v : values)
//		{
//			s += "\nChannel " + String(ch) + " : " + String(v);
//			ch++;
//		}
//		NLOG(niceName, s);
//	}
//
//
//	//dmxDevice->sendDMXRange(startChannel, values);
//}

//void DMXInterface::send16BitDMXValue(int startChannel, int value, DMXByteOrder byteOrder)
//{
//	if (!enabled->boolValue() || dmxDevice == nullptr) return;
//	if (logOutgoingData->boolValue()) NLOG(niceName, "Send 16-bit DMX : " + String(startChannel) + " > " + String(value));
//	//dmxDevice->sendDMXValue(startChannel, byteOrder == MSB ? (value >> 8) & 0xFF : value & 0xFF);
//	//dmxDevice->sendDMXValue(startChannel + 1, byteOrder == MSB ? 0xFF : (value >> 8) & 0xFF);
//
//}
//
//void DMXInterface::send16BitDMXValues(int startChannel, Array<int> values, DMXByteOrder byteOrder)
//{
//	if (!enabled->boolValue() || dmxDevice == nullptr) return;
//	if (logOutgoingData->boolValue()) NLOG(niceName, "Send 16-bit DMX : " + String(startChannel) + " > " + String(values.size()) + " values");
//
//	Array<int> dmxValues;
//	int numValues = values.size();
//	dmxValues.resize(numValues * 2);
//	for (int i = 0; i < numValues; ++i)
//	{
//		int value = values[i];
//		dmxValues.set(i * 2, byteOrder == MSB ? (value >> 8) & 0xFF : value & 0xFF);
//		dmxValues.set(i * 2 + 1, byteOrder == MSB ? 0xFF : (value >> 8) & 0xFF);
//	}
//
//	//dmxDevice->sendDMXRange(startChannel, dmxValues);
//}

void DMXInterface::dmxDeviceConnected()
{
	dmxConnected->setValue(true);
}

void DMXInterface::dmxDeviceDisconnected()
{
	dmxConnected->setValue(false);
}

void DMXInterface::dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName)
{
	if (isClearing || !enabled->boolValue()) return;
	if (logIncomingData->boolValue()) NLOG(niceName, "DMX In : Net " << net << ", Subnet " << subnet << ", Universe " << universe);

	dmxInterfaceListeners.call(&DMXInterfaceListener::dmxDataInChanged, net, subnet, universe, values, sourceName);
}


void DMXInterface::sendValuesForObjectInternal(Object* o)
{
	if (channelTestingMode->boolValue()) return;

	DMXParams* dmxParams = dynamic_cast<DMXParams*>(o->interfaceParameters.get());

	jassert(dmxParams != nullptr);

	//Fill map with active dmx channels from object manager
	int startChannel = dmxParams->startChannel->intValue();
	HashMap<int, float> compValues;

	for (auto& c : o->componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;
		c->fillOutValueMap(compValues, startChannel);
	}

	//Store these channels in local universe
	int net = dmxParams->net->enabled ? dmxParams->net->intValue() : defaultNet->intValue();
	int subnet = dmxParams->subnet->enabled ? dmxParams->subnet->intValue() : defaultSubnet->intValue();
	int universe = dmxParams->universe->enabled ? dmxParams->universe->intValue() : defaultUniverse->intValue();
	DMXUniverse* u = getUniverse(net, subnet, universe);

	HashMap<int, float>::Iterator it(compValues);
	while (it.next()) u->updateValue(it.getKey(), it.getValue() * 255);
}


DMXUniverse* DMXInterface::getUniverse(int net, int subnet, int universe, bool createIfNotExist)
{
	const int index = DMXUniverse::getUniverseIndex(net, subnet, universe);
	if (universeIdMap.contains(index)) return universeIdMap[index];

	if (!createIfNotExist) return nullptr;

	DMXUniverse* u = new DMXUniverse(net, subnet, universe);
	universes.add(u);
	universeIdMap.set(index, u);
	return universeIdMap[index];
}

void DMXInterface::run()
{
	double prevTime = Time::getMillisecondCounterHiRes();
	while (!threadShouldExit())
	{
		{
			GenericScopedLock lock(deviceLock);
			if (dmxDevice == nullptr) return;

			bool sendOnChange = sendOnChangeOnly->boolValue();
			for (auto& u : universes)
			{
				if (sendOnChange && !u->isDirty) continue;
				dmxDevice->sendDMXValues(u);
				if (logOutgoingData->boolValue())
				{
					NLOG(niceName, "Sending Universe " << u->toString());
				}
				u->isDirty = false;
			}
		}

		double t = Time::getMillisecondCounterHiRes();
		double diffTime = t - prevTime;
		double rateMS = 1000.0 / sendRate->intValue();
		prevTime = t;

		double msToWait = rateMS - diffTime;
		if (msToWait > 0) wait(msToWait);

	}
}

InterfaceUI* DMXInterface::createUI()
{
	return new DMXInterfaceUI(this);
}


// DMX PARAMS
DMXInterface::DMXParams::DMXParams() :
	ControllableContainer("DMX Params")
{
	net = addIntParameter("Net", "If appliccable the net for this universe", 0, 0, 15, false);
	subnet = addIntParameter("Subnet", "If applicable the subnet for this universe", 0, 0, 15, false);
	universe = addIntParameter("Universe", "The universe", 0, 0, false);

	net->canBeDisabledByUser = true;
	subnet->canBeDisabledByUser = true;
	universe->canBeDisabledByUser = true;

	startChannel = addIntParameter("Start Channel", "The first channel to set the values. The values will automatically distributed depending on the object.", 1, 1, 512);
}

