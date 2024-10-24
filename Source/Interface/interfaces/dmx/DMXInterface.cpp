/*
  ==============================================================================

	DMXInterface.cpp
	Created: 26 Sep 2020 1:51:42pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface/InterfaceIncludes.h"
#include "Object/ObjectIncludes.h"
#include "DMXInterface.h"

DMXInterface::DMXInterface() :
	Interface(getTypeString()),
	dmxInterfaceNotifier(20)
{

	dmxType = addEnumParameter("DMX Type", "Choose the type of dmx interface you want to connect");

	dmxType->addOption("Open DMX", DMXDevice::OPENDMX)->addOption("Enttec DMX Pro", DMXDevice::ENTTEC_DMXPRO)->addOption("Enttec DMX MkII", DMXDevice::ENTTEC_MK2)->addOption("Art-Net", DMXDevice::ARTNET)->addOption("sACN/E1.31", DMXDevice::SACN);
	dmxType->setValueWithKey("Open DMX");

	defaultNet = addIntParameter("Net", "If appliccable the net for this universe", 0, 0, 15, false);
	defaultSubnet = addIntParameter("Subnet", "If applicable the subnet for this universe", 0, 0, 15, false);
	defaultUniverse = addIntParameter("Universe", "The universe", 0, 0, 15, false);

	sendOnChangeOnly = addBoolParameter("Send On Change Only", "Only send a universe if one of its channels has changed", false);
	forceSendDefaultUniverse = addBoolParameter("Force Send Default Universe", "Force sending the default universe even if no objects are sending to it", true);

	channelTestingMode = addBoolParameter("Channel Testing Mode", "Is testing with the Channel view ?", false);
	channelTestingMode->hideInEditor = true;

	channelTestingFlashValue = addFloatParameter("Channel Testing Flash Value", "Flash value of channel testing", 1, 0, 1);
	channelTestingFlashValue->hideInEditor = true;

	setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
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
		if (dmxDevice != nullptr) dmxDevice->setEnabled(enabled->boolValue());
	}
	else if (p == dmxType)
	{
		setCurrentDMXDevice(DMXDevice::create((DMXDevice::Type)(int)dmxType->getValueData()));
	}
}

void DMXInterface::setCurrentDMXDevice(DMXDevice* d)
{
	if (dmxDevice.get() == d) return;

	GenericScopedLock lock(deviceLock);

	if (dmxDevice != nullptr)
	{
		dmxDevice->removeDMXDeviceListener(this);
		dmxDevice->clearDevice();
		removeChildControllableContainer(dmxDevice.get());
	}

	dmxDevice.reset(d);

	if (dmxDevice != nullptr)
	{
		dmxDevice->enabled = enabled->boolValue();
		dmxDevice->addDMXDeviceListener(this);
		addChildControllableContainer(dmxDevice.get());

		switch (dmxDevice->type)
		{
		case DMXDevice::OPENDMX:
		case DMXDevice::ENTTEC_DMXPRO:
		case DMXDevice::ENTTEC_MK2:
			defaultNet->setEnabled(false);
			defaultSubnet->setEnabled(false);
			defaultUniverse->setEnabled(false);
			break;

		case DMXDevice::ARTNET:
			defaultUniverse->setRange(0, 15);
			defaultNet->setEnabled(true);
			defaultSubnet->setEnabled(true);
			defaultUniverse->setEnabled(true);
			break;

		case DMXDevice::SACN:
			defaultUniverse->setRange(1, 63999);
			defaultNet->setEnabled(false);
			defaultSubnet->setEnabled(false);
			defaultUniverse->setEnabled(true);
			break;
		}
	}

	interfaceNotifier.addMessage(new InterfaceEvent(InterfaceEvent::DEVICE_CHANGED, this));
}

void DMXInterface::dmxDeviceSetupChanged(DMXDevice*)
{
	interfaceNotifier.addMessage(new InterfaceEvent(InterfaceEvent::DEVICE_CHANGED, this));
}

void DMXInterface::setDMXValue(int net, int subnet, int universe, int startChannel, Array<int> values)
{
	DMXUniverse* u = getUniverse(net, subnet, universe);
	for (int i = 0; i < values.size(); i++) u->updateValue(startChannel + i, values[i]);
}

void DMXInterface::dmxDataInChanged(DMXDevice*, int net, int subnet, int universe, Array<uint8> values, const String& sourceName)
{
	if (isClearing || !enabled->boolValue()) return;

	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "DMX In : Net " << net << ", Subnet " << subnet << ", Universe " << universe);

	dmxInterfaceListeners.call(&DMXInterfaceListener::dmxDataInChanged, net, subnet, universe, values, sourceName);
}


void DMXInterface::prepareSendValues()
{
	if (sendOnChangeOnly->boolValue() || channelTestingMode->boolValue()) return;

	universes.clear();
	universeIdMap.clear();

	if (forceSendDefaultUniverse->boolValue())  getUniverse(defaultNet->intValue(), defaultSubnet->intValue(), defaultUniverse->intValue(), true);

}

void DMXInterface::sendValuesForObjectInternal(Object* o)
{
	if (channelTestingMode->boolValue()) return;

	DMXParams* dmxParams = dynamic_cast<DMXParams*>(o->interfaceParameters.get());

	jassert(dmxParams != nullptr);

	int channelOffset = dmxParams->startChannel->intValue() - 1; //channelOffset is zero based to fill the universe array

	var params(new DynamicObject());
	params.getDynamicObject()->setProperty("channelOffset", channelOffset);

	//Store these channels in local universe
	int net = dmxParams->net->enabled ? dmxParams->net->intValue() : defaultNet->intValue();
	int subnet = dmxParams->subnet->enabled ? dmxParams->subnet->intValue() : defaultSubnet->intValue();
	int universe = dmxParams->universe->enabled ? dmxParams->universe->intValue() : defaultUniverse->intValue();
	DMXUniverse* u = getUniverse(net, subnet, universe);


	var channelsData;
	channelsData.resize(DMX_NUM_CHANNELS);
	for (int i = 0; i < u->values.size(); i++) channelsData[i] = u->values[i];

	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("channels", channelsData);

	for (auto& c : o->componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;
		c->fillInterfaceData(this, data, params);// , compValues, startChannel);
	}


	bool sOnChangeOnly = sendOnChangeOnly->boolValue();
	for (int i = 0; i < channelsData.size(); i++) u->updateValue(i, (int)channelsData[i], sOnChangeOnly);
}

void DMXInterface::finishSendValues()
{
	bool hasOneDirty = false;

	String sentUniverses = "";

	bool sendOnChange = sendOnChangeOnly->boolValue();
	bool log = logOutgoingData->boolValue();

	GenericScopedLock lock(deviceLock);
	if (dmxDevice == nullptr) return;
	
	{
		GenericScopedLock ulock(universes.getLock());
		for (auto& u : universes)
		{
			if (!u->isDirty && sendOnChange) continue;
			hasOneDirty |= u->isDirty;
			u->isDirty = false;

			if (log) sentUniverses += "\n" + u->toString();

			dmxDevice->setDMXValues(u);

			Array<uint8> values(u->values.getRawDataPointer(), u->values.size());
			dmxInterfaceNotifier.addMessage(new DMXInterfaceEvent(DMXInterfaceEvent::UNIVERSE_SENT, u, values));
		}
	}

	if(hasOneDirty || !sendOnChange) outActivityTrigger->trigger();
	if ((!sendOnChange || hasOneDirty) && log) NLOG(niceName, "Sending Universes : " + sentUniverses);
}



BoolParameter* DMXInterface::getConnectedParam()
{
	if (dmxDevice != nullptr) return dmxDevice->isConnected;
	return nullptr;
}

DMXUniverse* DMXInterface::getUniverse(int net, int subnet, int universe, bool createIfNotExist)
{
	const int index = DMXUniverse::getUniverseIndex(net, subnet, universe);
	if (universeIdMap.contains(index)) return universeIdMap[index];

	if (!createIfNotExist) return nullptr;

	DMXUniverse* u = new DMXUniverse(net, subnet, universe);
	universes.add(u);
	universeIdMap.set(index, u);
	return u;
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
	universe = addIntParameter("Universe", "The universe", 0, 0, 63999, false);

	net->canBeDisabledByUser = true;
	subnet->canBeDisabledByUser = true;
	universe->canBeDisabledByUser = true;

	startChannel = addIntParameter("Start Channel", "The first channel to set the values. The values will automatically distributed depending on the object.", 1, 1, 512);
}

