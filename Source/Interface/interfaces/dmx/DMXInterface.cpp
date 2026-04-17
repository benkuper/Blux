/*
  ==============================================================================

	DMXInterface.cpp
	Created: 26 Sep 2020 1:51:42pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface/InterfaceIncludes.h"
#include "Object/ObjectIncludes.h"

namespace
{
	struct DMXChannelMapping
	{
		int universeOffset = 0;
		int channel = 0;
	};

	void getUniverseAddressForOffset(DMXInterface* dmxInterface, int baseNet, int baseSubnet, int baseUniverse, int universeOffset, int& outNet, int& outSubnet, int& outUniverse)
	{
		if (dmxInterface != nullptr && dmxInterface->dmxDevice != nullptr && dmxInterface->dmxDevice->type == DMXDevice::ARTNET)
		{
			const int universeIndex = DMXUniverse::getUniverseIndex(baseNet, baseSubnet, baseUniverse) + universeOffset;
			outNet = (universeIndex >> 8) & 0x7f;
			outSubnet = (universeIndex >> 4) & 0xf;
			outUniverse = universeIndex & 0xf;
			return;
		}

		outNet = baseNet;
		outSubnet = baseSubnet;
		outUniverse = baseUniverse + universeOffset;
	}
}

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
	universeSplitMode = addEnumParameter("Universe Split Mode", "How values should be split when a DMX object spans across multiple universes");
	universeSplitMode->addOption("Channel-wise", CHANNEL_WISE)->addOption("Stride-aware", COMPONENT_STRIDE);
	universeSplitMode->setValueWithData(CHANNEL_WISE);

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

	var params(new DynamicObject());
 params.getDynamicObject()->setProperty("channelOffset", 0);

	//Store these channels in local universe
	int net = dmxParams->net->enabled ? dmxParams->net->intValue() : defaultNet->intValue();
	int subnet = dmxParams->subnet->enabled ? dmxParams->subnet->intValue() : defaultSubnet->intValue();
	int universe = dmxParams->universe->enabled ? dmxParams->universe->intValue() : defaultUniverse->intValue();
    const int channelOffset = dmxParams->startChannel->intValue() - 1;
	const bool strideAwareSplit = universeSplitMode->getValueDataAsEnum<UniverseSplitMode>() == COMPONENT_STRIDE;

	Array<ObjectComponent::DMXDataRange> dataRanges;
	int numChannels = 0;
	for (auto& c : o->componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;

		Array<ObjectComponent::DMXDataRange> componentRanges = c->getDMXDataRanges();
		for (auto& range : componentRanges)
		{
			if (range.numChannels <= 0) continue;
			dataRanges.add(range);
			numChannels = jmax(numChannels, range.startChannel + range.numChannels);
		}
	}

	if (numChannels <= 0) return;

	Array<int> groupLengths;
	groupLengths.resize(numChannels);
	for (int i = 0; i < numChannels; ++i) groupLengths.set(i, 1);

	if (strideAwareSplit)
	{
		for (auto& range : dataRanges)
		{
			const int rangeEnd = range.startChannel + range.numChannels;
			const int stride = jlimit(1, jmax(1, range.numChannels), range.splitStride);
			if (stride <= 1) continue;

			for (int splitStart = range.startChannel; splitStart < rangeEnd; splitStart += stride)
			{
				groupLengths.set(splitStart, jmax(groupLengths[splitStart], jmin(stride, rangeEnd - splitStart)));
			}
		}
	}

	Array<DMXChannelMapping> channelMappings;
	channelMappings.resize(numChannels);

	int currentUniverseOffset = 0;
	int currentChannel = channelOffset;
	for (int i = 0; i < numChannels; ++i)
	{
		const int groupLength = strideAwareSplit ? jmax(1, groupLengths[i]) : 1;
		const int remainingInUniverse = DMX_NUM_CHANNELS - currentChannel;
		if (groupLength > remainingInUniverse)
		{
			currentUniverseOffset++;
			currentChannel = 0;
		}

		DMXChannelMapping mapping;
		mapping.universeOffset = currentUniverseOffset;
		mapping.channel = currentChannel;
		channelMappings.set(i, mapping);

		currentChannel++;
		if (currentChannel >= DMX_NUM_CHANNELS)
		{
			currentUniverseOffset++;
			currentChannel = 0;
		}
	}

	HashMap<int, DMXUniverse*> universesByOffset;
	auto getMappedUniverse = [&](int universeOffset)
		{
			if (universesByOffset.contains(universeOffset)) return universesByOffset[universeOffset];

			int targetNet = net;
			int targetSubnet = subnet;
			int targetUniverse = universe;
			getUniverseAddressForOffset(this, net, subnet, universe, universeOffset, targetNet, targetSubnet, targetUniverse);

			DMXUniverse* mappedUniverse = getUniverse(targetNet, targetSubnet, targetUniverse);
			universesByOffset.set(universeOffset, mappedUniverse);
			return mappedUniverse;
		};

	var channelsData;
	channelsData.resize(numChannels);
	for (int i = 0; i < numChannels; ++i)
	{
		const DMXChannelMapping& mapping = channelMappings.getReference(i);
		DMXUniverse* mappedUniverse = getMappedUniverse(mapping.universeOffset);
		channelsData[i] = mappedUniverse->values[mapping.channel];
	}

	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("channels", channelsData);

	for (auto& c : o->componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;
		c->fillInterfaceData(this, data, params);// , compValues, startChannel);
	}


	bool sOnChangeOnly = sendOnChangeOnly->boolValue();
   for (int i = 0; i < channelsData.size(); i++)
	{
		const DMXChannelMapping& mapping = channelMappings.getReference(i);
		DMXUniverse* mappedUniverse = getMappedUniverse(mapping.universeOffset);
		mappedUniverse->updateValue(mapping.channel, (int)channelsData[i], sOnChangeOnly);
	}
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

