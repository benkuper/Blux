/*
  ==============================================================================

	ColorComponent.cpp
	Created: 26 Sep 2020 2:20:01pm
	Author:  bkupe

  ==============================================================================
*/

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
	if (sourceColors.size() != resolution->intValue())
	{
		sourceColors.resize(resolution->intValue());
		outColors.resize(resolution->intValue());
	}

	if (colorSource != nullptr) colorSource->fillColorsForObject(sourceColors, object, this);
	else sourceColors.fill(Colours::transparentBlack);
}

var ColorComponent::getOriginalComputedValues()
{
	var result;
	for (auto& i : sourceColors)
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

void ColorComponent::fillOutValueMap(HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset)
{
	int index = startChannel + (ignoreChannelOffset ? 0 : channelOffset);
	for (auto& c : outColors)
	{
		channelValueMap.set(index++, c.getFloatRed());
		channelValueMap.set(index++, c.getFloatGreen());
		channelValueMap.set(index++, c.getFloatBlue());
		//if(useAlpha) channelValueMap.set(index++, c.getFloatAlpha());
	}
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

var ColorComponent::getJSONData()
{
	var data = ObjectComponent::getJSONData();
	if (colorSource != nullptr)
	{
		data.getDynamicObject()->setProperty("colorSource", colorSource->getJSONData());
		if (colorSource->sourceTemplate) data.getDynamicObject()->setProperty("sourceTemplate", colorSource->sourceTemplate->shortName);
	}
	if (pixelShape != nullptr) data.getDynamicObject()->setProperty("pixelShape", pixelShape->getJSONData());
	return data;
}

void ColorComponent::loadJSONDataItemInternal(var data)
{
	ObjectComponent::loadJSONDataItemInternal(data);

	var csData = data.getProperty("colorSource", var());
	if (csData.isObject())
	{
		ColorSource* tc = ColorSourceLibrary::getInstance()->getItemWithName(data.getProperty("sourceTemplate", ""));
		setupSource(csData.getProperty("type", ""), tc);
		colorSource->loadJSONData(csData);
	}

	if (data.hasProperty("pixelShape"))
	{
		setupShape(data.getProperty("pixelShape", var()).getProperty("type", ""));
	}

	if (pixelShape != nullptr) data.getDynamicObject()->setProperty("pixelShape", pixelShape->getJSONData());
}

InspectableEditor* ColorComponent::getEditorInternal(bool isRoot)
{
	return new ColorComponentEditor(this, isRoot);
}
