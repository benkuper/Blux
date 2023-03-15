/*
  ==============================================================================

	CustomComponent.cpp
	Created: 15 Mar 2023 7:33:19pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

CustomComponent::CustomComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), CUSTOM, params)
{
	numValues = addIntParameter("Count", "Number of  values to use", 1, 1);
	rebuildValues();
}

CustomComponent::~CustomComponent()
{
}

void CustomComponent::onContainerParameterChangedInternal(Parameter* p)
{
	ObjectComponent::onContainerParameterChangedInternal(p);

	if (p == numValues)
	{
		rebuildValues();
	}
}

void CustomComponent::rebuildValues()
{
	while (computedParameters.size() > numValues->intValue())
	{
		removeComputedParameter(computedParameters.getLast());
	}

	while (computedParameters.size() < numValues->intValue())
	{
		FloatParameter* v = new FloatParameter("Value " + String(computedParameters.size() + 1), "", 0, 0, 1);
		v->userCanChangeName = true;
		v->isCustomizableByUser = true;
		addComputedParameter(v);
	}

	Interface* i = (Interface*)object->targetInterface->targetContainer.get();
	rebuildInterfaceParams(i);
}

var CustomComponent::getJSONData()
{
	var data = ObjectComponent::getJSONData();
	var valueNames;
	var valueRanges;
	for (auto& sp : sourceParameters)
	{
		valueNames.append(sp->niceName);
		var r;
		r.append(sp->minimumValue);
		r.append(sp->maximumValue);
		valueRanges.append(r);
	}
	data.getDynamicObject()->setProperty("valueNames", valueNames);
	data.getDynamicObject()->setProperty("valueRanges", valueRanges);

	return data;
}

void CustomComponent::loadJSONDataItemInternal(var data)
{
	ObjectComponent::loadJSONDataItemInternal(data);
	
	var vData = data["valueNames"];
	var rData = data["valueRanges"];
	for (int i = 0; i < sourceParameters.size() && i < vData.size(); i++)
	{
		sourceParameters[i]->setNiceName(vData[i]);
		if (i < rData.size()) sourceParameters[i]->setRange(rData[i][0],rData[i][1]);
	}
}
