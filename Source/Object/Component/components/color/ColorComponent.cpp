/*
  ==============================================================================

	ColorComponent.cpp
	Created: 26 Sep 2020 2:20:01pm
	Author:  bkupe

  ==============================================================================
*/

#include "ColorComponent.h"
#include "Color/ColorSource/ColorSource.h"
#include "Color/ColorSource/ColorSourceFactory.h"
#include "Color/PixelShape/PixelShape.h"
#include "ui/ColorComponentEditor.h"
#include "Color/ColorSource/sources/pattern/PatternColorSource.h"

ColorComponent::ColorComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), COLOR, params)
{
	resolution = addIntParameter("Resolution", "Number of different colors/pixels for this object", 1, 1);
}

ColorComponent::~ColorComponent()
{
}

void ColorComponent::setupSource(const String& type, ColorSource* templateRef)
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
		addChildControllableContainer(colorSource.get());
	}
}

void ColorComponent::setupShape()
{
	pixelShape.reset(new CirclePixelShape(resolution->intValue()));
}

void ColorComponent::setupFromJSONDefinition(var definition)
{
	ObjectComponent::setupFromJSONDefinition(definition);

	int res = definition.getProperty("resolution", 1);
	bool resolutionIsEditable = definition.getProperty("resolutionIsEditable", true);
	resolution->setControllableFeedbackOnly(!resolutionIsEditable);
	resolution->defaultValue = res;
	resolution->resetValue(true);

	setupShape();

	String defaultSource = definition.getProperty("defaultSource", "Solid Color");
	setupSource(defaultSource);

	update();
}

void ColorComponent::update()
{
	if (colors.size() != resolution->intValue()) colors.resize(resolution->intValue());
	if (colorSource != nullptr) colorSource->fillColorsForObject(colors, object, this);
}

var ColorComponent::getOriginalComputedValues()
{
	var result;
	for (auto& i : colors)
	{
		var c;
		c.append(i.getFloatRed());
		c.append(i.getFloatGreen());
		c.append(i.getFloatBlue());
		c.append(i.getFloatAlpha());
		result.append(c);
	}

	return result;
}

void ColorComponent::onContainerParameterChangedInternal(Parameter* p)
{
	ObjectComponent::onContainerParameterChangedInternal(p);

	if (p == resolution)
	{
		if (pixelShape != nullptr) pixelShape->resolution = resolution->intValue();
		update();
	}
}

InspectableEditor* ColorComponent::getEditor(bool isRoot)
{
	return new ColorComponentEditor(this, isRoot);
}
