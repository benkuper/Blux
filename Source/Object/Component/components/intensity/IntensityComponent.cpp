#include "IntensityComponent.h"
/*
  ==============================================================================

	IntensityComponent.cpp
	Created: 26 Sep 2020 2:20:14pm
	Author:  bkupe

  ==============================================================================
*/

IntensityComponent::IntensityComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), INTENSITY, params),
	curve("Remap Curve")
{
	curve.setCanBeDisabled(true);
	curve.enabled->setValue(false);
	curve.addKey(0, 0);
	curve.addKey(1, 1);
	addChildControllableContainer(&curve);
}

IntensityComponent::~IntensityComponent()
{
}


void IntensityComponent::postProcessComponentValues(var& values)
{
	for (int i = 0; i < values.size(); i++)
	{
		values[i] = curve.getValueAtPosition(values[i]);
	}
}

void IntensityComponent::setupFromJSONDefinition(var data)
{
	ObjectComponent::setupFromJSONDefinition(data);

	if (!data.hasProperty("channels"))
	{
		var chData(new DynamicObject());
		chData.getDynamicObject()->setProperty("Value", 0);
		data.getDynamicObject()->setProperty("channels", chData);
	}

	NamedValueSet chData = data.getProperty("channels", var()).getDynamicObject()->getProperties();
	for (auto& ch : chData)
	{
		String s = ch.name.toString();
		FloatParameter* f = addFloatParameter(s, "Value of the intensity. This will automatically be converted to 0-255 or whatever the output protocol is.", 0, 0, 1);
		sourceParameters.add(f);
		addComputedParameter(new FloatParameter("Out " + s, "Computed intensity " + s + " after all effects applied", 0, 0, 1), ch.value, f);
		values.add(f);
	}


}
