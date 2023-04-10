/*
  ==============================================================================

	AutomationEffect.cpp
	Created: 4 Oct 2020 11:37:20pm
	Author:  bkupe

  ==============================================================================
*/


#include "Effect/EffectIncludes.h"

AutomationEffect::AutomationEffect(var params) :
	TimedEffect(getTypeString(), params),
	automation("Curve")
{
	length = effectParams.addFloatParameter("Length", "Length of this automation", 2, .1f);
	length->defaultUI = FloatParameter::TIME;
	length->canBeAutomated = false;

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

	effectParams.addChildControllableContainer(&automation);

}

AutomationEffect::~AutomationEffect()
{
}

void AutomationEffect::processComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time, float originalTime)
{
	if (c->mainParameter == nullptr) return;

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

	float val = automation.getValueAtPosition(relTime / _length);
	
	targetValues.set(c->mainParameter, val);
}

void AutomationEffect::effectParamChanged(Controllable* c)
{
	TimedEffect::effectParamChanged(c);

	if (c == range)
	{
		automation.valueRange->setPoint(range->getPoint());
	}
	else if (c == loop)
	{
		if (!loop->boolValue())
		{
			//force put curTime in 0-length range to have good ending behaviour
			HashMap<ObjectComponent*, float>::Iterator it(curTimes);
			while (it.next())
			{
				Object* o = it.getKey()->object;
				int id = o->globalID->intValue();
				curTimes.set(it.getKey(), fmodf(it.getValue(), GetLinkedValueT(length, 0)));
			}
		}
	}
}