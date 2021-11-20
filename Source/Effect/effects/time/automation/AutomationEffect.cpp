/*
  ==============================================================================

	AutomationEffect.cpp
	Created: 4 Oct 2020 11:37:20pm
	Author:  bkupe

  ==============================================================================
*/

AutomationEffect::AutomationEffect(var params) :
	TimedEffect(getTypeString(), params),
	automation("Curve")
{
	length = effectParams.addFloatParameter("Length", "Length of this automation", 2, .1f);
	length->defaultUI = FloatParameter::TIME;

	controllables.swap(controllables.indexOf(length), controllables.indexOf(sceneSaveMode) + 1); //length first

	clipTime = effectParams.addBoolParameter("Clip Time", "If checked, this will force negative time with positive speed to clip. Useful for starting animations when loading scenes", false);
	loop = effectParams.addBoolParameter("Loop", "If checked, this will loop the automation. If not, this will go up until the end and stay there (taking care of time offsets)", true);
	range = effectParams.addPoint2DParameter("Range", "Range of this automation");

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

	filterManager->componentSelector.allowedComponents.removeAllInstancesOf(ComponentType::COLOR);
	filterManager->componentSelector.selectedComponents.set(ComponentType::INTENSITY, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::PANTILT, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::SERVO, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::STEPPER, true);
}

AutomationEffect::~AutomationEffect()
{
}

var AutomationEffect::getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, var value, int id, float time)
{
	float _length = GetLinkedValue(length);
	float _speed = GetLinkedValue(speed);

	float relTime = (bool)GetLinkedValue(loop) ? fmodf(time, _length) : jmin(time, _length);

	if (!(bool)GetLinkedValue(clipTime))
	{
		if (relTime < 0) relTime += _length;
	}
	else
	{
		if (relTime < 0 && _speed > 0) relTime = 0;
		else if (_speed < 0)
		{
			if (relTime > 0) relTime = _length;
			else relTime += _length;
		}
	}

	return automation.getValueAtPosition(relTime / _length);
}

void AutomationEffect::onContainerParameterChangedInternal(Parameter* p)
{
	TimedEffect::onContainerParameterChangedInternal(p);

	if (p == range)
	{
		automation.valueRange->setPoint(range->getPoint());
	}
	else if (p == loop)
	{
		if (!loop->boolValue())
		{
			//force put curTime in 0-length range to have good ending behaviour
			HashMap<ObjectComponent*, float>::Iterator it(curTimes);
			while (it.next())
			{
				Object* o = it.getKey()->object;
				int id = o->globalID->intValue();
				curTimes.set(it.getKey(), fmodf(it.getValue(), GetLinkedValue(length)));
			}
		}
	}
}