/*
  ==============================================================================

    ColorSource.cpp
    Created: 2 Nov 2020 6:13:15pm
    Author:  bkupe

  ==============================================================================
*/
#include "ColorSource.h"
#include "Object/Component/components/color/ColorComponent.h"

ColorSource::ColorSource(const String& name, var params) :
    BaseItem(name, false)
{
	itemDataType = "ColorSource";
}

ColorSource::~ColorSource()
{
}

Array<Colour> ColorSource::getColorsForObject(Object* o, ColorComponent* c, int id, float time)
{
    Array<Colour> result;
    result.resize(c->resolution->intValue());
    fillColorsForObject(result, o, c, id, time);
    return result;
}

void ColorSource::fillColorsForObject(Array<Colour>& colors, Object* o, ColorComponent* c, int id, float time)
{
    colors.fill(Colours::black);
}

TimedColorSource::TimedColorSource(const String& name, var params)
{
	speed = addFloatParameter("Speed", "The speed at which play this", 1);
	timeOffset = addFloatParameter("Time Offset", "This allows for offsetting the time, for manual position animation for example.", 0);
	timeOffset->defaultUI = FloatParameter::TIME;

	offsetByID = addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);
}

TimedColorSource::~TimedColorSource()
{
}

void TimedColorSource::fillColorsForObject(Array<Colour>& colors, Object* o, ColorComponent* c, int id, float time)
{
	float targetTime = getCurrentTime(time) - offsetByID->floatValue() * id + timeOffset->floatValue();
	fillColorsForObjectTimeInternal(colors, o, c, id, targetTime);
}


float TimedColorSource::getCurrentTime(float timeOverride)
{
	return timeOverride >= 0 ? timeOverride : curTime;
}

void TimedColorSource::timerCallback()
{
	if (!enabled->boolValue()) return;
	addTime();
}

void TimedColorSource::addTime()
{
	double newTime = Time::getMillisecondCounter() / 1000.0;
	curTime += (newTime - timeAtLastUpdate) * speed->floatValue();
	timeAtLastUpdate = newTime;
}

void TimedColorSource::onContainerParameterChangedInternal(Parameter* p)
{
	ColorSource::onContainerParameterChangedInternal(p);
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
