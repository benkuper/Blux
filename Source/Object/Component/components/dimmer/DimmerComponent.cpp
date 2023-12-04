/*
  ==============================================================================

	DimmerComponent.cpp
	Created: 26 Sep 2020 2:20:14pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"
#include "Interface/InterfaceIncludes.h"

DimmerComponent::DimmerComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), DIMMER, params),
	curve("Remap Curve")
{

	saveAndLoadRecursiveData = true;
	value = (FloatParameter*)addComputedParameter(new FloatParameter("Value", "Value of the dimmer", 0, 0, 1));

	curve.editorCanBeCollapsed = true;
	curve.setCanBeDisabled(true);
	curve.enabled->setValue(false);

	curve.addKey(0, 0);
	curve.addKey(1, 1);
	addChildControllableContainer(&curve);

	useFineValue = addBoolParameter("Use Fine Value", "If checked, the value will be split into 2 channels, one for the integer part and one for the decimal part", false);
}

DimmerComponent::~DimmerComponent()
{
}


void DimmerComponent::updateComputedValues(HashMap<Parameter*, var>& values)
{
	if (curve.enabled->boolValue())
	{
		Parameter* compValue = paramComputedMap[value];
		float val = values[compValue];
		curve.position->setValue(val);
		float endVal = curve.value->floatValue();
		values.set(compValue, endVal);
	}

	ObjectComponent::updateComputedValues(values);
}

void DimmerComponent::fillInterfaceData(Interface* i, var data, var params)
{
	ObjectComponent::fillInterfaceData(i, data, params);

	if (ObjectManager::getInstance()->blackOut->boolValue()) return;

	if (DMXInterface* di = dynamic_cast<DMXInterface*>(i))
	{
		int channelOffset = params.getProperty("channelOffset", 0);
		var channelsData = data.getProperty("channels", var());

		Parameter* channelP = computedInterfaceMap[paramComputedMap[value]];
		if (channelP == nullptr || !channelP->enabled) return;
		int channel = channelP->intValue();
		int targetChannel = channelOffset + channel - 1; //convert local channel to 0-based


		float pVal = (float)channelsData[targetChannel];
		channelsData[targetChannel] = floor(pVal);
		if (useFineValue->boolValue() && targetChannel + 1 < channelsData.size()) channelsData[targetChannel + 1] = fmodf(pVal, 1) * 255;
	}
}
