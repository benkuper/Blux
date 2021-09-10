/*
  ==============================================================================

	ColorSourceOverrideEffect.cpp
	Created: 9 Nov 2020 12:54:23pm
	Author:  bkupe

  ==============================================================================
*/

ColorSourceOverrideEffect::ColorSourceOverrideEffect(var params) :
	ColorEffect("Override (Color)"),
	overrideEffectNotifier(5)
{
	filterManager->componentSelector.allowedComponents.clear();
	filterManager->componentSelector.allowedComponents.add(ComponentType::COLOR);

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
		removeChildControllableContainer(colorSource.get());
	}

	ColorSource* cs = ColorSourceFactory::getInstance()->create(type);
	colorSource.reset(cs);

	if (colorSource != nullptr)
	{
		if (templateRef != nullptr) cs->linkToTemplate(templateRef);
		addChildControllableContainer(colorSource.get(), false, 0);
	}

	overrideEffectNotifier.addMessage(new OverrideEffectEvent(OverrideEffectEvent::SOURCE_CHANGED, this));
}

void ColorSourceOverrideEffect::processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
	if (colorSource == nullptr) return;
	colorSource->fillColorsForObject(colors, o, c, id, time);
}

var ColorSourceOverrideEffect::getJSONData()
{
	var data = ColorEffect::getJSONData();
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

InspectableEditor* ColorSourceOverrideEffect::getEditor(bool isRoot)
{
	return new ColorSourceOverrideEffectEditor(this, isRoot);
}
