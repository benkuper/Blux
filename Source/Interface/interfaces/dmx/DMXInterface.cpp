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
	Thread("DMX Interface"),
	dmxInterfaceNotifier(20)
{
	dmxType = addEnumParameter("DMX Type", "Choose the type of dmx interface you want to connect");

	dmxType->addOption("Open DMX", DMXDevice::OPENDMX)->addOption("Enttec DMX Pro", DMXDevice::ENTTEC_DMXPRO)->addOption("Enttec DMX MkII", DMXDevice::ENTTEC_MK2)->addOption("Art-Net", DMXDevice::ARTNET)->addOption("sACN/E1.31", DMXDevice::SACN);
	dmxType->setValueWithKey("Open DMX");

	dmxConnected = addBoolParameter("Connected", "DMX is connected ?", false);
	dmxConnected->isControllableFeedbackOnly = true;
	dmxConnected->isSavable = false;

	defaultNet = addIntParameter("Net", "If appliccable the net for this universe", 0, 0, 15, false);
	defaultSubnet = addIntParameter("Subnet", "If applicable the subnet for this universe", 0, 0, 15, false);
	defaultUniverse = addIntParameter("Universe", "The universe", 0, 0, 15, false);

	sendRate = addIntParameter("Send Rate", "The rate at which to send data.", 40, 1, 200);
	sendOnChangeOnly = addBoolParameter("Send On Change Only", "Only send a universe if one of its channels has changed", false);


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

	if (enabled->boolValue() && dmxDevice != nullptr) startThread();
}

void DMXInterface::setDMXValue(int net, int subnet, int universe, int startChannel, Array<int> values)
{
	DMXUniverse* u = getUniverse(net, subnet, universe);
	for (int i = 0; i < values.size(); i++) u->updateValue(startChannel + i, values[i]);
}

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


void DMXInterface::prepareSendValues()
{
	if (sendOnChangeOnly->boolValue() || channelTestingMode->boolValue()) return;
	for (auto& u : universes) u->values.fill(0); // reset all universes to 0
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
		//if (c == colorComp) continue; //colorComp is only set if useIntensityForColor is set
		//if (colorComp != nullptr && c == ic && intensityForColor) continue;
		c->fillInterfaceData(this, data, params);// , compValues, startChannel);
	}


	//HashMap<int, float>::Iterator it(compValues);

	//if (logOutgoingData->boolValue()) NLOG(niceName, "Updating " << compValues.size() << " values");

	bool sOnChangeOnly = sendOnChangeOnly->boolValue();

	for (int i = 0; i < channelsData.size(); i++)
	{
		if (logOutgoingData->boolValue()) NLOG(niceName, String(i + 1) << " : " << (int)(channelsData[i]));
		u->updateValue(i, (int)channelsData[i], sOnChangeOnly);
	}
}

void DMXInterface::finishSendValues()
{
	GenericScopedLock lock(universesToSend.getLock());
	universesToSend.clear();
	for (auto& u : universes)
	{
		universesToSend.add(new DMXUniverse(u));
		u->isDirty = false;
	}
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

	while (!threadShouldExit())
	{
		double loopStartTime = Time::getMillisecondCounterHiRes();

		{
			//if (dmxDevice == nullptr) return;

			bool sendOnChange = sendOnChangeOnly->boolValue();

			GenericScopedLock lock(universesToSend.getLock());
			for (auto& u : universesToSend)
			{
				if (sendOnChange && !u->isDirty) continue;
				{
					GenericScopedLock lock(deviceLock);
					if (dmxDevice != nullptr) dmxDevice->sendDMXValues(u);
				}


				if (logOutgoingData->boolValue())
				{
					NLOG(niceName, "Sending Universe " << u->toString());
				}

				Array<uint8> values(u->values.getRawDataPointer(), u->values.size());
				dmxInterfaceNotifier.addMessage(new DMXInterfaceEvent(DMXInterfaceEvent::UNIVERSE_SENT, u, values));

			}
		}


		double t = Time::getMillisecondCounterHiRes();
		double diffTime = t - loopStartTime;
		double rateMS = 1000.0 / sendRate->intValue();

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
	universe = addIntParameter("Universe", "The universe", 0, 0, 63999, false);

	net->canBeDisabledByUser = true;
	subnet->canBeDisabledByUser = true;
	universe->canBeDisabledByUser = true;

	startChannel = addIntParameter("Start Channel", "The first channel to set the values. The values will automatically distributed depending on the object.", 1, 1, 512);
}

