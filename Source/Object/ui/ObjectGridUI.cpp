/*
  ==============================================================================

    ObjectGridUI.cpp
    Created: 26 Sep 2020 7:51:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectGridUI.h"
#include "UI/AssetManager.h"
#include "../ObjectManager.h"
#include "../Component/components/intensity/IntensityComponent.h"
#include "../Component/components/color/ColorComponent.h"
#include "ChainViz/ChainViz.h"

ObjectGridUI::ObjectGridUI(Object* object) :
	BaseItemMinimalUI(object),
	shouldRepaint(false),
	transparentBG(false),
    previewIntensity(0),
	iconIntensityRef(nullptr),
    flashMode(false)
{

	updateThumbnail();
	bringToFrontOnSelect = false;
	autoHideWhenDragging = false;

	if (IntensityComponent* ic = item->getComponent<IntensityComponent>())
	{
		iconIntensityRef = (FloatParameter*)ic->computedParameters[0];
		computedIntensityUI.reset(iconIntensityRef->createSlider());
		computedIntensityUI->useCustomBGColor = true;
		computedIntensityUI->customBGColor = BG_COLOR.darker(.6f);
		computedIntensityUI->showLabel = false;
		computedIntensityUI->showValue = false;
		addAndMakeVisible(computedIntensityUI.get());


		intensityUI.reset(((FloatParameter*)ic->values[0])->createSlider());
		intensityUI->useCustomBGColor = true;
		intensityUI->customBGColor = BG_COLOR.darker(.6f);
		intensityUI->showLabel = false;
		intensityUI->showValue = false;
		addAndMakeVisible(intensityUI.get());
	}

	if (ColorComponent* cc = item->getComponent<ColorComponent>())
	{
		computedColorUI.reset((ColorParameterUI *)((ColorParameter*)cc->computedParameters[0]->createDefaultUI()));
		addAndMakeVisible(computedColorUI.get());
	}

	globalIDUI.reset(item->globalID->createLabelUI());
	addAndMakeVisible(globalIDUI.get());

	setRepaintsOnMouseActivity(true);

	setSize(128, 128);

	startTimerHz(10);
}

ObjectGridUI::~ObjectGridUI()
{
	//if (item != nullptr) item->removeAsyncModelListener(this);
}

void ObjectGridUI::paint(Graphics& g)
{
	g.setColour(bgColor);
	if(!transparentBG) g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);

	Rectangle<int> r = getLocalBounds();

	
	r.removeFromBottom(20);

	if (!previewData.isVoid())
	{
		Rectangle<float> pr = r.withHeight(10).withBottomY(r.getBottom()).reduced(2).toFloat();
		g.setColour(Colours::purple.darker(1));
		g.fillRoundedRectangle(pr, 4);
		g.setColour(Colours::purple.brighter(.3f));
		g.fillRoundedRectangle(pr.withWidth(previewIntensity * pr.getWidth()), 4);
		g.setColour(Colours::purple.brighter());
		g.drawRoundedRectangle(pr, 4, .5f);
	}

	
	float overMultiplier = isMouseOver() ? .4f : 1.f;

	float offMult = 1;
	if (iconIntensityRef != nullptr && objectONImage.isValid())
	{
		offMult = 1 - iconIntensityRef->floatValue();
	}
	g.setColour(Colours::white.withAlpha(overMultiplier));
	g.drawImage(objectImage, r.reduced(6).toFloat(), RectanglePlacement::centred);
	if (iconIntensityRef != nullptr && objectONImage.isValid())
	{
		g.setColour(Colours::white.withAlpha((1-offMult) * overMultiplier));
		g.drawImage(objectONImage, r.reduced(6).toFloat(), RectanglePlacement::centred);
	}

	if (!objectImage.isValid() || isMouseOver())
	{
		g.setColour(Colours::white);
		g.drawFittedText(item->niceName, getLocalBounds().reduced(4), Justification::centred, 3);
	}
}

void ObjectGridUI::resized()
{
	Rectangle<int> r = getLocalBounds();

	globalIDUI->setVisible(r.getWidth() >= 90);
	if(globalIDUI->isVisible()) globalIDUI->setBounds(r.withSize(40, 16).reduced(2));

	if (computedColorUI != nullptr)
	{
		Rectangle<int> cr = getLocalBounds().removeFromRight(30).removeFromTop(30).reduced(4);
		computedColorUI->setBounds(cr);
	}
	
	if (computedIntensityUI != nullptr)
	{
		computedIntensityUI->setBounds(r.removeFromBottom(10).reduced(2));
		intensityUI->setBounds(r.removeFromBottom(10).reduced(2));
	}
}


void ObjectGridUI::updateThumbnail()
{
	var iconData = item->icon->getValueData();

	File img1;
	File img2;
	
	if (iconData.isArray())
	{
		img1 = File(iconData[0].toString());
		img2 = File(iconData[1].toString());
	}else
	{
		img1 = File(iconData.toString());
	}
	
	if (img1.existsAsFile()) objectImage = ImageCache::getFromFile(img1);
	if (img2.existsAsFile()) objectONImage = ImageCache::getFromFile(img2);

	if (!objectImage.isValid()) objectImage = BluxAssetManager::getImage("icon128");

	shouldRepaint = true;
}

void ObjectGridUI::setPreviewData(var data)
{
	if (item->excludeFromScenes->boolValue()) return;
	if (previewData.isVoid() && data.isVoid()) return; //Avoid repainting all the time

	previewData = data.clone();
	var iData = previewData.getProperty("components", var()).getProperty("intensity", var()).getProperty("value", var());
	if (!iData.isVoid()) previewIntensity = (float)iData;

	repaint();
}

void ObjectGridUI::mouseDown(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseDown(e);

	if (e.mods.isAltDown())
	{
		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipValueRef = o->slideManipParameter->floatValue();
			if (e.mods.isRightButtonDown()) if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(ObjectManager::getInstance()->defaultFlashValue->floatValue());
		}
	}
}

void ObjectGridUI::mouseDrag(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseDrag(e);

	if (e.mods.isAltDown())
	{
		const float pixelRange = 200;
		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(o->slideManipValueRef - e.getDistanceFromDragStartY() / pixelRange);
		}
	}
}

void ObjectGridUI::mouseUp(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseUp(e);

	if (e.mods.isAltDown())
	{
		Array<Object*> objects;
		if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
		objects.addIfNotAlreadyThere(item);
		
		if (e.mods.isRightButtonDown())
		{
			for (auto& o : objects)
			{
				if (o->slideManipParameter == nullptr) continue;
				o->slideManipParameter->setValue(o->slideManipValueRef);
			}
		}
		else
		{
			Array<UndoableAction*> actions;
			for (auto& o : objects) if (o->slideManipParameter != nullptr) actions.add(o->slideManipParameter->setUndoableValue(o->slideManipValueRef, o->slideManipParameter->floatValue(), true));
			UndoMaster::getInstance()->performActions("Change " + String(actions.size()) + " values ", actions);
		}
	}
}

void ObjectGridUI::mouseDoubleClick(const MouseEvent& e)
{
	ShapeShifterManager::getInstance()->showContent(ChainViz::panelName);
	if (ChainViz* viz = ShapeShifterManager::getInstance()->getContentForType<ChainViz>())
	{
		viz->setCurrentObject(item);
	}
}

bool ObjectGridUI::keyStateChanged(bool isDown)
{
	if (isDown)
	{
		if (KeyPress::isKeyCurrentlyDown(KeyPress::createFromDescription("f").getKeyCode()))
		{
			flashMode = true;

			Array<Object*> objects;
			if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
			objects.addIfNotAlreadyThere(item);
			for (auto& o : objects)
			{
				if (o->slideManipParameter != nullptr) o->slideManipValueRef = o->slideManipParameter->floatValue();
				if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(ObjectManager::getInstance()->defaultFlashValue->floatValue());
			}

			return true;
		}else if (KeyPress::isKeyCurrentlyDown(KeyPress::createFromDescription("v").getKeyCode()))
		{
			ShapeShifterManager::getInstance()->showContent(ChainViz::panelName);
			if (ChainViz* viz = ShapeShifterManager::getInstance()->getContentForType<ChainViz>())
			{
				viz->setCurrentObject(item);
			}
			return true;
		}
	}
	else
	{
		if (flashMode)
		{
			Array<Object*> objects;
			if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
			objects.addIfNotAlreadyThere(item);
			for (auto& o : objects)
			{
				if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(o->slideManipValueRef);
			}

			flashMode = false;
			return true;
		}
	}

	return false;
}

void ObjectGridUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	if (IntensityComponent* ic = c->getParentAs<IntensityComponent>())
	{
		shouldRepaint = true;
	}
	else if (c == item->icon)
	{
		updateThumbnail();
		shouldRepaint = true;
	}
}

void ObjectGridUI::timerCallback()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}
