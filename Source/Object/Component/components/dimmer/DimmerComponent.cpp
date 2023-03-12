/*
  ==============================================================================

	DimmerComponent.cpp
	Created: 26 Sep 2020 2:20:14pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

DimmerComponent::DimmerComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), DIMMER, params),
	curve("Remap Curve")
{

	saveAndLoadRecursiveData = true;

	value = addFloatParameter("Value", "Valuer of the dimmer", 0, 0, 1);
	curve.setCanBeDisabled(true);
	curve.enabled->setValue(false);
	curve.addKey(0, 0);
	curve.addKey(1, 1);
	addChildControllableContainer(&curve);
}

DimmerComponent::~DimmerComponent()
{
}


void DimmerComponent::updateComputedValues(HashMap<Parameter*, var>& values)
{
	if (!curve.enabled->boolValue()) return;

	Parameter* compValue = computedParamMap[value];
	float val = values[compValue];
	float endVal = curve.getValueAtPosition(val);
	values.set(compValue, endVal);

	ObjectComponent::updateComputedValues(values);
}

void DimmerComponent::setupFromJSONDefinition(var data)
{
}
