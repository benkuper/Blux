/*
  ==============================================================================

	BentoInterface.cpp
	Created: 29 Sep 2020 3:56:02pm
	Author:  bkupe

  ==============================================================================
*/

BentoInterface::BentoInterface() :
	Interface(getTypeString())
{
	multiStripMode = addBoolParameter("Multi-strip Support", "If checked, this will prepend one byte containing the strip index to the udp color packet", true);
}

BentoInterface::~BentoInterface()
{
}

void BentoInterface::sendValuesForObjectInternal(Object* o)
{
	if (ColorComponent* colorComp = o->getComponent<ColorComponent>())
	{
		int numLeds = colorComp->resolution->intValue();

		HashMap<int, float> valueMap;
		float fac = 1;
		if (DimmerComponent* ic = o->getComponent<DimmerComponent>())
		{
			//ic->fillOutValueMap(valueMap, 0, true);
			fac = valueMap[0];
		}

		//colorComp->fillOutValueMap(valueMap, 0, true);

		BentoInterfaceParams* bParams = dynamic_cast<BentoInterfaceParams*>(o->interfaceParameters.get());
		
		Array<uint8_t> data;
		if (multiStripMode->boolValue()) data.add(bParams->stripIndex->intValue());
		
		for (int i = 0; i < numLeds; i++)
		{
			int index = i * 3;
			data.add(valueMap[index] * fac * 254);
			data.add(valueMap[index + 1] * fac * 254);
			data.add(valueMap[index + 2] * fac * 254);
		}

		data.add(255);

		int dataSent = ledSender.write(bParams->remoteHost->stringValue(), remoteLedPort, data.getRawDataPointer(), data.size());
		if (dataSent == -1)
		{
			NLOGWARNING(niceName, "Could not send data to " << bParams->remoteHost->stringValue());
		}

		if (logOutgoingData->boolValue())
		{
			NLOG(niceName, "Sent " << dataSent << " bytes");
		}
	}
}


BentoInterface::BentoInterfaceParams::BentoInterfaceParams() :
	ControllableContainer("Interface Parameters")
{
	remoteHost = addStringParameter("Remote Host", "IP of the prop on the network", "192.168.0.100");
	//sendRate = addIntParameter("Send Rate", "Frequency at which to send the colors", 40, 1, 200);
	stripIndex = addIntParameter("Strip Index", "The index of the strip to control", 1, 1, 8);
}
