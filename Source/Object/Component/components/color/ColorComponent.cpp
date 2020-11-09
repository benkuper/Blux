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
	ObjectComponent(o, getTypeString(), COLOR, params),
	colorComponentNotifier(5)
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

	colorComponentNotifier.addMessage(new ColorComponentEvent(ColorComponentEvent::SOURCE_CHANGED, this));
}

void ColorComponent::setupShape(const String& type)
{
	if (type == "Line") pixelShape.reset(new LinePixelShape(resolution->intValue()));
	else if (type == "Circle") pixelShape.reset(new CirclePixelShape(resolution->intValue()));
	else pixelShape.reset(new PointPixelShape(resolution->intValue()));

	colorComponentNotifier.addMessage(new ColorComponentEvent(ColorComponentEvent::SHAPE_CHANGED, this));
}

void ColorComponent::setupFromJSONDefinition(var definition)
{
	ObjectComponent::setupFromJSONDefinition(definition);

	int res = definition.getProperty("resolution", 1);
	bool resolutionIsEditable = definition.getProperty("resolutionIsEditable", true);
	resolution->setControllableFeedbackOnly(!resolutionIsEditable);
	resolution->defaultValue = res;
	resolution->resetValue(true);

	String defaultShape = "";

	var shapeData = definition.getProperty("shape", var());
	if (shapeData.isObject()) defaultShape = shapeData.getProperty("type", "");
	
	setupShape(defaultShape);

	pixelShape->loadJSONData(shapeData);

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
