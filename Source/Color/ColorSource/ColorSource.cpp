/*
  ==============================================================================

    ColorSource.cpp
    Created: 2 Nov 2020 6:13:15pm
    Author:  bkupe

  ==============================================================================
*/


ColorSource::ColorSource(const String& name, var params) :
	BaseItem(name, false),
	sourceTemplate(nullptr)
{
	saveAndLoadRecursiveData = true;
	itemDataType = "ColorSource";
}

ColorSource::~ColorSource()
{
	linkToTemplate(nullptr);
}

void ColorSource::linkToTemplate(ColorSource* st)
{
	if (sourceTemplate != nullptr)
	{
		sourceTemplate->removeInspectableListener(this);
		sourceTemplate->removeControllableContainerListener(this);
	}

	sourceTemplate = st;
	sourceTemplateRef = st;

	if (sourceTemplate != nullptr)
	{
		jassert(sourceTemplate->getTypeString() == getTypeString());

		sourceTemplate->addInspectableListener(this);
		sourceTemplate->addControllableContainerListener(this);

		for (auto& c : sourceTemplate->controllables)
		{
			if (Parameter* p = dynamic_cast<Parameter*>(c))
			{
				if (Parameter* sp = getParameterByName(p->shortName))
				{
					if (!sp->isOverriden)
					{
						sp->defaultValue = p->value;
						sp->resetValue();
					}
				}
			}
		}
	}
}

void ColorSource::inspectableDestroyed(Inspectable* i)
{
	if (i == sourceTemplateRef) linkToTemplate(nullptr);
}

String ColorSource::getSourceLabel() const
{
	return sourceTemplate != nullptr? ("[T] "+sourceTemplate->niceName):getTypeString();
}

void ColorSource::fillColorsForObject(Array<Colour, CriticalSection> & colors, Object* o, ColorComponent* c, int id, float time)
{
	if (id == -1) id = o->globalID->intValue();
	fillColorsForObjectInternal(colors, o, c, id, time);
}

void ColorSource::fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
	colors.fill(Colours::black);
}

void ColorSource::controllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	if (cc == sourceTemplate)
	{
		if (Parameter* sp = dynamic_cast<Parameter*>(getControllableForAddress(c->getControlAddress(cc))))
		{
			if (!sp->isOverriden)
			{
				sp->defaultValue = ((Parameter*)c)->value;
				sp->resetValue();
			}
		}
	}
	else
	{
		BaseItem::controllableFeedbackUpdate(cc, c);
	}
}




TimedColorSource::TimedColorSource(const String& name, var params) :
	ColorSource(name, params),
	curTime(0)
{
	speed = addFloatParameter("Speed", "The speed at which play this", .5f);
	timeOffset = addFloatParameter("Time Offset", "This allows for offsetting the time, for manual position animation for example.", 0);
	timeOffset->defaultUI = FloatParameter::TIME;

	offsetByID = addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);

	timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
	curTime = 0;
	startTimer(50);
}

TimedColorSource::~TimedColorSource()
{
}

void TimedColorSource::linkToTemplate(ColorSource* st)
{
	ColorSource::linkToTemplate(st);

	if (sourceTemplate == nullptr)
	{
		speed->hideInEditor = false;
		speed->setControllableFeedbackOnly(false);
		startTimer(50);
	}
	else
	{
		speed->hideInEditor = true;
		speed->setControllableFeedbackOnly(true);
		speed->resetValue();
		stopTimer();
	}
}

void TimedColorSource::fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
	float targetTime = getCurrentTime(time) - offsetByID->floatValue() * id + timeOffset->floatValue();
	fillColorsForObjectTimeInternal(colors, o, c, id, targetTime);
}


float TimedColorSource::getCurrentTime(float timeOverride)
{
	if (sourceTemplate != nullptr && !sourceTemplateRef.wasObjectDeleted()) return ((TimedColorSource*)sourceTemplate)->getCurrentTime();

	return timeOverride >= 0 ? timeOverride*speed->floatValue() : curTime;
}

void TimedColorSource::hiResTimerCallback()
{
	addTime();
}

void TimedColorSource::addTime()
{
	double newTime = Time::getMillisecondCounter() / 1000.0;
	curTime += (newTime - timeAtLastUpdate) * speed->floatValue();
	timeAtLastUpdate = newTime;
}
