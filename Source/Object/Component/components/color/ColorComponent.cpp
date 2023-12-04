/*
  ==============================================================================

	ColorComponent.cpp
	Created: 26 Sep 2020 2:20:01pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"
#include "Interface/InterfaceIncludes.h"

bool ColorComponent::isChangingMainColor = false;

ColorComponent::ColorComponent(Object* o, var params) :
	ObjectComponent(o, getTypeString(), COLOR, params),
	dimmerComponent(nullptr),
	colorComponentNotifier(5)
{
	resolution = addIntParameter("Resolution", "Number of different colors/pixels for this object", 1, 1);
	useDimmerForOpacity = addBoolParameter("Use Dimmer for Opacity", "If checked, use the dimmer component for color opacity", false);

	colorMode = addEnumParameter("Color Mode", "Color mode for this object");
	colorMode->addOption("RGB", RGB)->addOption("RGBW", RGBW)->addOption("WRGB", WRGB)->addOption("RGBAW", RGBAW)->addOption("RGBWA", RGBWA);

	fineMode = addEnumParameter("Fine Mode", "Fine Color Mode for this object. None means not using fine channels. Alternate means R/R fine, G/G fine, etc. Follow means R/G/B/R fine/G fine/ B fine, etc.");
	fineMode->addOption("None", None)->addOption("Alternate", Alternate)->addOption("Follow", Follow);

	whiteTemperature = addFloatParameter("White Temperature", "Temperature of the white color in Kelvin", 6500, 2000, 12000);
	whiteTemperature->unitSteps = 1.0f / 100;

	mainColor = (ColorParameter*)addComputedParameter(new ColorParameter("Main Color", "Computed main color, not used to send DMX but for feedback", Colours::black), nullptr, false);
	mainColor->setControllableFeedbackOnly(true);
	mainColor->hideInEditor = true;

	update();
}

ColorComponent::~ColorComponent()
{
}

void ColorComponent::setupSource(const String& type, ColorSource* templateRef)
{
	if (colorSource != nullptr)
	{
		Array<WeakReference<Parameter>> sourceParams = colorSource->sourceParams.getAllParameters(true);
		for (auto& p : sourceParams) sceneDataParameters.removeAllInstancesOf(p);

		removeChildControllableContainer(colorSource.get());
	}

	ColorSource* cs = ColorSourceFactory::getInstance()->create(type);
	colorSource.reset(cs);

	if (colorSource != nullptr)
	{
		if (templateRef != nullptr) cs->linkToTemplate(templateRef);
		addChildControllableContainer(colorSource.get());

		Array<WeakReference<Parameter>> sourceParams = colorSource->sourceParams.getAllParameters(true);
		for (auto& p : sourceParams) sceneDataParameters.addIfNotAlreadyThere(p);
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

void ColorComponent::fillComputedValueMap(HashMap<Parameter*, var>& values)
{
	var colors;
	for (auto& i : sourceColors)
	{
		var c;
		c.append(i.getFloatRed());
		c.append(i.getFloatGreen());
		c.append(i.getFloatBlue());
		c.append(i.getFloatAlpha());
		colors.append(c);
	}

	values.set(nullptr, colors); //using nullptr as placeholders for values not linked to a computed parameter
}

void ColorComponent::updateComputedValues(HashMap<Parameter*, var>& values)
{
	var colValues = values[nullptr].clone(); //using nullptr as placeholders for values not linked to a computed parameter

	jassert(colValues.size() == resolution->intValue());
	jassert(colValues[0].size() >= 4);


	if (ObjectManager::getInstance()->blackOut->boolValue())
	{
		var zeroVal;
		zeroVal.append(0);
		zeroVal.append(0);
		zeroVal.append(0);
		zeroVal.append(0);
		colValues.getArray()->fill(zeroVal);
		outColors.fill(Colours::black);
	}
	else
	{
		float mult = 1;
		if (useDimmerForOpacity->boolValue())
		{
			if (dimmerComponent == nullptr)
			{
				dimmerComponent = dynamic_cast<DimmerComponent*>(object->getComponentForType(ComponentType::DIMMER));
			}
			if (dimmerComponent != nullptr) mult = dimmerComponent->mainParameter->floatValue();
		}

		for (int i = 0; i < colValues.size(); i++)
		{
			if (colValues[i].size() < 4) continue;
			var col;
			col.append((float)colValues[i][0] * mult);
			col.append((float)colValues[i][1] * mult);
			col.append((float)colValues[i][2] * mult);
			col.append((float)colValues[i][3] * mult);

			outColors.set(i, Colour::fromFloatRGBA(col[0], col[1], col[2], col[3]));
		}
	}


	if (colValues.size() > 0 && colValues[0].size() >= 4)
	{
		paramComputedMap[mainColor]->setValue(colValues[0]);
	}
}


void ColorComponent::fillInterfaceDataInternal(Interface* i, var data, var params)
{
	if (DMXInterface* di = dynamic_cast<DMXInterface*>(i))
	{
		int channelOffset = params.getProperty("channelOffset", 0);
		var channelsData = data.getProperty("channels", var());

		Parameter* channelP = computedInterfaceMap[paramComputedMap[mainColor]];
		if (channelP == nullptr || !channelP->enabled) return;
		int channel = channelP->intValue();
		int targetChannel = channelOffset + channel - 1; //convert local channel to 0-based

		ColorMode cm = (ColorMode)colorMode->intValue();
		const int* indices = colorModeIndices[(int)cm];

		FineMode fm = fineMode->getValueDataAsEnum<FineMode>();

		int colorSize = cm == RGB ? 3 : (cm == RGBW || cm == WRGB) ? 4 : 5;

		int temp = whiteTemperature->intValue();

		for (int i = 0; i < outColors.size(); i++)
		{
			var c;
			if (cm == RGBW || cm == WRGB) c = ColorHelpers::getRGBWFromRGB(outColors[i], temp);
			else if (cm == RGBWA || cm == RGBAW) c = ColorHelpers::getRGBWAFromRGB(outColors[i], temp);
			else
			{
				c.append(outColors[i].getFloatRed());
				c.append(outColors[i].getFloatGreen());
				c.append(outColors[i].getFloatBlue());
			}

			int ch = targetChannel + i * colorSize;

			for (int ci = 0; ci < colorSize; ci++)
			{
				if (ch + ci >= channelsData.size()) break;

				switch (fm)
				{
				case None:
					channelsData[ch + ci] = roundToInt((float)c[indices[ci]] * 255);
					break;

				case Alternate:
				case Follow:
				{
					int index1 = fm == Alternate ? ch + ci * 2 : ch + ci;
					int index2 = fm == Alternate ? index1 + 1 : index1 + colorSize;

					if (index2 >= channelsData.size()) break;

					float val = (float)c[indices[ci]] * 255;

					channelsData[index1] = floor(val);
					channelsData[index2] = fmodf(val, 1) * 255;

				}
				break;
				}
			}
		}

		return;
	}

	ObjectComponent::fillInterfaceDataInternal(i, data, params);
}

//void ColorComponent::fillOutValueMap(HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset)
//{
//	int index = startChannel + (ignoreChannelOffset ? 0 : channelOffset);
//	for (auto& c : outColors)
//	{
//		channelValueMap.set(index++, c.getFloatRed());
//		channelValueMap.set(index++, c.getFloatGreen());
//		channelValueMap.set(index++, c.getFloatBlue());
//		//if(useAlpha) channelValueMap.set(index++, c.getFloatAlpha());
//	}
//}

void ColorComponent::onContainerParameterChangedInternal(Parameter* p)
{
	ObjectComponent::onContainerParameterChangedInternal(p);

	if (p == resolution)
	{
		if (pixelShape != nullptr) pixelShape->resolution = resolution->intValue();
		update();
	}
}

void ColorComponent::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	ObjectComponent::onControllableFeedbackUpdateInternal(cc, c);


	if (!isChangingMainColor && colorSource != nullptr)
	{
		if (ColorParameter* cp = colorSource->getMainColorParameter())
		{
			if (c == cp)
			{
				isChangingMainColor = true;
				Colour col = cp->getColor();
				Array<Object*> selObjects = InspectableSelectionManager::mainSelectionManager->getInspectablesAs<Object>();
				for (auto& o : selObjects)
				{
					if(o == object) continue;
					if (ColorComponent* cc = dynamic_cast<ColorComponent*>(o->getComponentForType(ComponentType::COLOR)))
					{
						if (ColorParameter* ccp = cc->colorSource->getMainColorParameter())
						{
							ccp->setColor(col);
						}
					}
				}
				isChangingMainColor = false;
			}
		}
		
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

InspectableEditor* ColorComponent::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ColorComponentEditor(this, isRoot);
}
