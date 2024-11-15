/*
  ==============================================================================

	ObjectGridUI.cpp
	Created: 26 Sep 2020 7:51:18pm
	Author:  bkupe

  ==============================================================================
*/

#include "ChainViz/ChainViz.h"
#include "Color/ColorIncludes.h"
#include "Object/ObjectIncludes.h"

juce_ImplementSingleton(ObjectUITimer)

ObjectGridUI::ObjectGridUI(Object* object) :
	BaseItemMinimalUI(object),
	shouldRepaint(false),
	transparentBG(false),
	previewIntensity(0),
	iconIntensityRef(nullptr),
	flashMode(false)
{
	highlightOnMouseOver = true;
	updateThumbnail();
	bringToFrontOnSelect = false;
	autoHideWhenDragging = false;

	item->componentManager->addAsyncManagerListener(this);

	globalIDUI.reset(item->globalID->createLabelUI());
	globalIDUI->showLabel = false;
	addAndMakeVisible(globalIDUI.get());

	setRepaintsOnMouseActivity(true);

	setSize(128, 128);

	updateUI();

	ObjectUITimer::getInstance()->registerUI(this);
}

ObjectGridUI::~ObjectGridUI()
{
	if (ObjectUITimer* t = ObjectUITimer::getInstanceWithoutCreating()) t->unregisterUI(this);
	if (item != nullptr)
	{
		item->componentManager->removeAsyncManagerListener(this);
	}

	if (ColorComponent* colorComp = item->getComponent<ColorComponent>())
	{
		colorComp->removeAsyncColorComponentListener(this);
	}
}

void ObjectGridUI::paint(Graphics& g)
{
	if (!isVisible()) return;

	if (!transparentBG)
	{
		g.setColour(isMouseOver() ? bgColor.brighter(.05f) : bgColor);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	}

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


	if (colorViz == nullptr)
	{

		float offMult = 1;
		if (iconIntensityRef != nullptr && objectONImage.isValid())
		{
			offMult = 1 - iconIntensityRef->floatValue();
		}

		Rectangle<float> imgR = r.reduced(6).toFloat();
		g.setColour(Colours::white);
		g.drawImage(objectImage, imgR.reduced(2), RectanglePlacement::centred);

		if (iconIntensityRef != nullptr && objectONImage.isValid())
		{
			g.setColour(Colours::white.withAlpha((1 - offMult)));
			g.drawImage(objectONImage, imgR.reduced(2), RectanglePlacement::centred);
		}

		if (ColorComponent* colorComp = item->getComponent<ColorComponent>())
		{
			if (colorComp->colorSource != nullptr)
			{
				g.setColour(colorComp->mainColor->getColor());
				g.drawRoundedRectangle(imgR, 2, 2);
			}
		}
	}

}

void ObjectGridUI::paintOverChildren(Graphics& g)
{
	if (!isVisible()) return;

	if (!objectImage.isValid() || isMouseOver() || ObjectManager::getInstance()->alwaysShowNamesInUI->boolValue())
	{
		g.setColour(bgColor.darker().withAlpha(.5f));
		g.fillRoundedRectangle(getLocalBounds().withSizeKeepingCentre(getWidth() * .7f, getHeight() * .3f).toFloat(), 4);
		
		g.setColour(Colours::white.withAlpha(isMouseOver()?.8f:.5f));
		g.drawFittedText(item->niceName, getLocalBounds().reduced(4), Justification::centred, 3);
	}


	BaseItemMinimalUI::paintOverChildren(g);
}

void ObjectGridUI::resized()
{
	Rectangle<int> r = getLocalBounds();

	if (globalIDUI == nullptr) return;

	globalIDUI->setVisible(r.getWidth() >= 100);
	if (globalIDUI->isVisible()) globalIDUI->setBounds(r.withSize(40, 16).reduced(2));



	if (computedIntensityUI != nullptr)
	{
		computedIntensityUI->setBounds(r.removeFromBottom(10).reduced(2));
		intensityUI->setBounds(r.removeFromBottom(10).reduced(2));
	}

	if (colorViz != nullptr)
	{
		colorViz->setBounds(r.reduced(6));
	}

	if (mainColorUI != nullptr) mainColorUI->setBounds(getLocalBounds().removeFromRight(16).removeFromTop(16));

}


void ObjectGridUI::updateUI()
{
	if (ColorComponent* colorComp = item->getComponent<ColorComponent>())
	{
		if (!ObjectManager::getInstance()->showIconForColor->boolValue())
		{
			if (colorViz == nullptr)
			{
				colorViz.reset(new ColorViz(colorComp));
				addAndMakeVisible(colorViz.get());
			}
		}
		else if (colorViz != nullptr)
		{
			removeChildComponent(colorViz.get());
			colorViz.reset();
		}

		if (mainColorUI != nullptr) removeChildComponent(mainColorUI.get());
		mainColorUI.reset();
		if (colorComp->colorSource != nullptr)
		{
			if (ColorParameter* cp = colorComp->colorSource->getMainColorParameter())
			{
				mainColorUI.reset(cp->createColorParamUI());
				addAndMakeVisible(mainColorUI.get());
			}
		}

		colorComp->addAsyncColorComponentListener(this);
	}
	else if (colorViz != nullptr)
	{
		mainColorUI.reset();
		removeChildComponent(colorViz.get());
		colorViz.reset();
	}


	if (DimmerComponent* ic = item->getComponent<DimmerComponent>())
	{
		if (computedIntensityUI == nullptr)
		{

			iconIntensityRef = (FloatParameter*)ic->paramComputedMap[ic->value];
			jassert(iconIntensityRef != nullptr);
			computedIntensityUI.reset(iconIntensityRef->createSlider());
			computedIntensityUI->useCustomBGColor = true;
			computedIntensityUI->customBGColor = BG_COLOR.darker(.6f);
			computedIntensityUI->showLabel = false;
			computedIntensityUI->showValue = false;
			addAndMakeVisible(computedIntensityUI.get());


			intensityUI.reset(((FloatParameter*)ic->value)->createSlider());
			intensityUI->useCustomBGColor = true;
			intensityUI->customBGColor = BG_COLOR.darker(.6f);
			intensityUI->showLabel = false;
			intensityUI->showValue = false;
			addAndMakeVisible(intensityUI.get());
		}
	}
	else if (computedIntensityUI != nullptr)
	{
		removeChildComponent(computedIntensityUI.get());
		removeChildComponent(intensityUI.get());
		computedIntensityUI.reset();
		intensityUI.reset();
	}

	resized();
	repaint();
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
	}
	else if (iconData.isString())
	{
		img1 = File(iconData.toString());
	}
	else if (iconData.isInt() && (int)iconData == -1)
	{
		img1 = item->customIcon->getFile();
		if (img1.existsAsFile())
		{
			StringArray fSplit;
			fSplit.addTokens(img1.getFileNameWithoutExtension(), "_", "\"");
			File offFile = img1.getParentDirectory().getChildFile(fSplit[0] + "_off" + img1.getFileExtension());
			if (offFile.existsAsFile())
			{
				img2 = img1;
				img1 = offFile;
			}
		}

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
	var iData = previewData.getProperty("components", var()).getProperty("dimmer", var()).getProperty("value", var());
	if (!iData.isVoid()) previewIntensity = (float)iData;

	repaint();
}

void ObjectGridUI::mouseDown(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
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
	else if (e.mods.isRightButtonDown())
	{
		PopupMenu m;
		ColorSourceMenu cm;
		m.addSubMenu("Set Color Source", cm);

		m.showMenuAsync(PopupMenu::Options(), [this](int result)
			{
				if (result == 0) return;

				ColorSource* refColorSource = result < -1 ? ColorSourceLibrary::getInstance()->items[result + 10000] : nullptr;
				String type = refColorSource != nullptr ? refColorSource->getTypeString() : (result > 0 ? ColorSourceFactory::getInstance()->defs[result - 1]->type : "");

				Array<Object*> objects;
				if (item->isSelected) objects.addArray(InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>());
				objects.addIfNotAlreadyThere(item);

				for (auto& o : objects)
				{
					if (ColorComponent* c = o->getComponent<ColorComponent>())
					{
						c->setupSource(type, refColorSource);
					}
				}
			}
		);
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
			for (auto& o : objects) if (o->slideManipParameter != nullptr) actions.addArray(o->slideManipParameter->setUndoableValue(o->slideManipParameter->floatValue(), true));
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
		}
		else if (KeyPress::isKeyCurrentlyDown(KeyPress::createFromDescription("v").getKeyCode()))
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
	if (DimmerComponent* ic = c->getParentAs<DimmerComponent>())
	{
		shouldRepaint = true;
	}
	else if (ColorComponent* cc = c->getParentAs<ColorComponent>())
	{
		if (colorViz == nullptr && c == cc->mainColor)
		{
			shouldRepaint = true;
		}
	}
	else if (c == item->icon || c == item->customIcon)
	{
		updateThumbnail();
		shouldRepaint = true;
	}
}

void ObjectGridUI::visibilityChanged()
{
	BaseItemMinimalUI::visibilityChanged();
	if (colorViz != nullptr) colorViz->setVisible(isVisible());
}

void ObjectGridUI::newMessage(const ComponentManager::ManagerEvent& e)
{
	updateUI();
}

void ObjectGridUI::newMessage(const ColorComponent::ColorComponentEvent& e)
{
	if (e.type == ColorComponent::ColorComponentEvent::SOURCE_CHANGED)
	{
		updateUI();
	}
}

void ObjectGridUI::handleRepaint()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}



ObjectUITimer::ObjectUITimer()
{
	startTimerHz(20);
}

void ObjectUITimer::registerUI(ObjectGridUI* ui)
{
	uis.addIfNotAlreadyThere(ui);
}

void ObjectUITimer::unregisterUI(ObjectGridUI* ui)
{
	uis.removeAllInstancesOf(ui);
}

void ObjectUITimer::timerCallback()
{
	for (auto& ui : uis) ui->handleRepaint();
}
