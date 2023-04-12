/*
  ==============================================================================

	ColorSource.cpp
	Created: 2 Nov 2020 6:13:15pm
	Author:  bkupe

  ==============================================================================
*/

#include "Color/ColorIncludes.h"
#include "Object/ObjectIncludes.h"

ColorSource::ColorSource(const String& name, var params) :
	BaseItem(name, false),
	sourceParams("Parameters"),
	sourceTemplate(nullptr)
{

	saveAndLoadRecursiveData = true;
	itemDataType = "ColorSource";

	sourceParams.hideEditorHeader = true;
	sourceParams.editorCanBeCollapsed = false;
	sourceParams.addParamLinkContainerListener(this);
	sourceParams.saveAndLoadRecursiveData = true;
	addChildControllableContainer(&sourceParams);

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

void ColorSource::paramControlModeChanged(ParamLinkContainer* pc, ParameterLink* pl)
{
	colorSourceListeners.call(&ColorSourceListener::colorSourceParamControlModeChanged, pl->parameter);
}

void ColorSource::inspectableDestroyed(Inspectable* i)
{
	if (i == sourceTemplateRef) linkToTemplate(nullptr);
}

Colour ColorSource::getLinkedColor(ColorParameter* p, Object* o, int id, float originalTime)
{
	var cVar = GetSourceLinkedValue(p);
	return Colour::fromFloatRGBA(cVar[0], cVar[1], cVar[2], cVar[3]);
}

String ColorSource::getSourceLabel() const
{
	return sourceTemplate != nullptr ? ("[T] " + sourceTemplate->niceName) : getTypeString();
}

void ColorSource::fillColorsForObject(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
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
	speed = sourceParams.addFloatParameter("Speed", "The speed at which play this", .5f);
	timeOffset = sourceParams.addFloatParameter("Time Offset", "This allows for offsetting the time, for manual position animation for example.", 0);
	timeOffset->defaultUI = FloatParameter::TIME;

	offsetByID = sourceParams.addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);

	timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
	curTime = 0;
	
	TimedEffectHiResTimer::getInstance()->addTimerListener(this);
}

TimedColorSource::~TimedColorSource()
{
	if(TimedEffectHiResTimer::getInstanceWithoutCreating() != nullptr) TimedEffectHiResTimer::getInstance()->removeTimerListener(this);

}

void TimedColorSource::linkToTemplate(ColorSource* st)
{
	ColorSource::linkToTemplate(st);

	if (sourceTemplate == nullptr)
	{
		speed->hideInEditor = false;
		speed->setControllableFeedbackOnly(false);
		if (TimedEffectHiResTimer::getInstanceWithoutCreating() != nullptr) TimedEffectHiResTimer::getInstance()->addTimerListener(this);
	}
	else
	{
		speed->hideInEditor = true;
		speed->setControllableFeedbackOnly(true);
		speed->resetValue();
		if (TimedEffectHiResTimer::getInstanceWithoutCreating() != nullptr) TimedEffectHiResTimer::getInstance()->removeTimerListener(this);
	}
}

void TimedColorSource::fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
	float targetTime = getCurrentTime(time) - offsetByID->floatValue() * id + timeOffset->floatValue();
	fillColorsForObjectTimeInternal(colors, o, c, id, targetTime, time);
}


float TimedColorSource::getCurrentTime(float timeOverride)
{
	if (sourceTemplate != nullptr && !sourceTemplateRef.wasObjectDeleted()) return ((TimedColorSource*)sourceTemplate)->getCurrentTime();

	return timeOverride >= 0 ? timeOverride * speed->floatValue() : curTime;
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
