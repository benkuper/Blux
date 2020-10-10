/*
  ==============================================================================

	AutomationEffect.cpp
	Created: 4 Oct 2020 11:37:20pm
	Author:  bkupe

  ==============================================================================
*/

#include "AutomationEffect.h"

AutomationEffect::AutomationEffect(var params) :
	TimedEffect(getTypeString(), params),
	automation("Curve")
{
	length = addFloatParameter("Length", "Length of this automation", 2, .1f);
	length->defaultUI = FloatParameter::TIME;

	controllables.swap(controllables.indexOf(length), controllables.indexOf(sceneSaveMode)+1); //length first

	clipTime = addBoolParameter("Clip Time", "If checked, this will force negative time with positive speed to clip. Useful for starting animations when loading scenes", false);

	range = addPoint2DParameter("Range", "Range of this automation");

	var val;
	val.append(0);
	val.append(1);
	range->defaultValue = val;
	range->resetValue();

	automation.isSelectable = false;
	automation.length->setValue(1);
	automation.addKey(0, 0, false);
	automation.items[0]->easingType->setValueWithData(Easing::BEZIER);
	automation.addKey(.5f, 1, false);
	automation.items[0]->easingType->setValueWithData(Easing::BEZIER);
	automation.addKey(1, 0, false);
	automation.selectItemWhenCreated = false;
	automation.rangeRemapMode->setValueWithData(Automation::RangeRemapMode::PROPORTIONAL);

	addChildControllableContainer(&automation);

	filterManager.componentSelector.selectedComponents.set(ObjectComponent::INTENSITY, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::PANTILT, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::SERVO, true);
	filterManager.componentSelector.selectedComponents.set(ObjectComponent::STEPPER, true);
}

AutomationEffect::~AutomationEffect()
{
}

var AutomationEffect::getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, int id, var value, float time)
{
	float relTime = fmodf(time, length->floatValue());

	if (!clipTime->boolValue())
	{
		if (relTime < 0) relTime += length->floatValue();
	}
	else
	{
		if (relTime < 0 && speed->floatValue() > 0) relTime = 0;
		else if (speed->floatValue() < 0)
		{
			if (relTime > 0) relTime = length->floatValue();
			else relTime += length->floatValue();
		}
	}

	return automation.getValueAtPosition(relTime / length->floatValue());
}

void AutomationEffect::onContainerParameterChangedInternal(Parameter* p)
{
	TimedEffect::onContainerParameterChangedInternal(p);

	if (p == range)
	{
		automation.valueRange->setPoint(range->getPoint());
	}
}