/*
  ==============================================================================

	ColorSourceOverrideEffect.cpp
	Created: 9 Nov 2020 12:54:23pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

ColorSourceOverrideEffect::ColorSourceOverrideEffect(var params) :
	ColorEffect(getTypeString(), params),
	overrideEffectNotifier(5)
{
	fillWithOriginalColors = false;
	setupSource("Solid Color");
}

ColorSourceOverrideEffect::~ColorSourceOverrideEffect()
{
}

void ColorSourceOverrideEffect::setupSource(const String& type, ColorSource* templateRef)
{
	if (colorSource != nullptr)
	{
		effectParams.removeChildControllableContainer(colorSource.get());
	}

	effectParams.clear();

	ColorSource* cs = ColorSourceFactory::getInstance()->create(type);
	colorSource.reset(cs);

	if (colorSource != nullptr)
	{
		if (templateRef != nullptr) cs->linkToTemplate(templateRef);
		effectParams.addChildControllableContainer(colorSource.get(), false, 0);
		colorSource->addColorSourceListener(this);
	}

	overrideEffectNotifier.addMessage(new OverrideEffectEvent(OverrideEffectEvent::SOURCE_CHANGED, this));
}

void ColorSourceOverrideEffect::processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
	if (colorSource == nullptr) return;
	colorSource->fillColorsForObject(colors, o, c, id, time);
}

void ColorSourceOverrideEffect::colorSourceParamControlModeChanged(Parameter* p)
{
	effectListeners.call(&EffectListener::effectParamControlModeChanged, p);
}

var ColorSourceOverrideEffect::getJSONData(bool includeNonOverriden)
{
	var data = ColorEffect::getJSONData(includeNonOverriden);
	if (colorSource != nullptr)
	{
		data.getDynamicObject()->setProperty("colorSource", colorSource->getJSONData());
		if (colorSource->sourceTemplate) data.getDynamicObject()->setProperty("sourceTemplate", colorSource->sourceTemplate->shortName);
	}
	return data;
}

void ColorSourceOverrideEffect::loadJSONDataItemInternal(var data)
{
	ColorEffect::loadJSONDataItemInternal(data);

	var csData = data.getProperty("colorSource", var());
	if (csData.isObject())
	{
		ColorSource* tc = ColorSourceLibrary::getInstance()->getItemWithName(data.getProperty("sourceTemplate", ""));
		setupSource(csData.getProperty("type", ""), tc);
		colorSource->loadJSONData(csData);
	}
}

InspectableEditor* ColorSourceOverrideEffect::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ColorSourceOverrideEffectEditor(this, isRoot);
}
