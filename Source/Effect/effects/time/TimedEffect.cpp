/*
  ==============================================================================

    TimedEffect.cpp
    Created: 9 Oct 2020 9:16:14pm
    Author:  bkupe

  ==============================================================================
*/

#include "TimedEffect.h"

TimedEffect::TimedEffect(const String &name, var params) :
    Effect(name, params),
    timeAtLastUpdate(Time::getMillisecondCounter() / 1000.0),
    curTime(0)
{
    speed = addFloatParameter("Speed", "The speed at which play this", 1);
	offsetByID = addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);
	offsetByValue = addFloatParameter("Time Offset By Value", "Time Offset by parameter inside a component", 0);
}

TimedEffect::~TimedEffect()
{
}

var TimedEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values)
{

	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].isArray()) continue;
		float time = curTime - offsetByID->floatValue() * id - offsetByValue->floatValue() * i;
		values[i] = getProcessedComponentValueTimeInternal(o, c, id, values[i], time);
	}

	return values;
}

void TimedEffect::timerCallback()
{
    if (!enabled->boolValue()) return;
    addTime();
}

void TimedEffect::addTime()
{
    double newTime = Time::getMillisecondCounter() / 1000.0;
    curTime += (newTime - timeAtLastUpdate) * speed->floatValue();
    timeAtLastUpdate = newTime;

}

void TimedEffect::onContainerParameterChangedInternal(Parameter* p)
{
	Effect::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue())
		{
			timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
			curTime = 0;
			startTimer(20);
		}
		else
		{
			stopTimer();
		}
	}
}
