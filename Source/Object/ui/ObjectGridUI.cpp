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

ObjectGridUI::ObjectGridUI(Object* object) :
	BaseItemMinimalUI(object)
{
	//bgColor = item->isBeingEdited ? BLUE_COLOR.darker().withSaturation(.3f) : bgColor = BG_COLOR.brighter(.1f);

	setSize(128,128);
	updateThumbnail();

	autoHideWhenDragging = false;


	setRepaintsOnMouseActivity(true);
	//object->addAsyncModelListener(this);
}

ObjectGridUI::~ObjectGridUI()
{
	//if (item != nullptr) item->removeAsyncModelListener(this);
}

void ObjectGridUI::paint(Graphics& g)
{
	g.setColour(bgColor);
	g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	
	Rectangle<int> r = getLocalBounds();

	if (IntensityComponent* ic = item->getComponent<IntensityComponent>())
	{
		Rectangle<float> ir = r.removeFromBottom(10).reduced(2).toFloat();
		
		g.setColour(YELLOW_COLOR.darker(.2f));
		g.fillRoundedRectangle(ir.withWidth(ir.proportionOfWidth(ic->value->floatValue())),4);

		g.setColour(YELLOW_COLOR.brighter(.2f));
		g.drawRoundedRectangle(ir, 4, .5f);

		ir = r.removeFromBottom(10).reduced(2).toFloat();

		g.setColour(BLUE_COLOR.darker(.2f));
		g.fillRoundedRectangle(ir.withWidth(ir.proportionOfWidth(ic->computedParameters[0]->floatValue())), 4);

		g.setColour(BLUE_COLOR.brighter(.2f));
		g.drawRoundedRectangle(ir, 4, .5f);
	}

	g.setColour(Colours::white.withAlpha(isMouseOver() ? .2f : 1.f));
	if (objectImage.getWidth() > 0) g.drawImage(objectImage, r.reduced(6).toFloat(), RectanglePlacement::centred);

	if (objectImage.getWidth() == 0 || isMouseOver())
	{
		g.setColour(Colours::white);
		g.drawFittedText(item->niceName, getLocalBounds().reduced(4), Justification::centred, 3);
	}

}


void ObjectGridUI::updateThumbnail()
{
	if (item->customThumbnailPath.existsAsFile()) objectImage = ImageCache::getFromFile(item->customThumbnailPath);
	if (objectImage.getWidth() == 0) objectImage = BluxAssetManager::getImage("icon128");

	repaint();
}

void ObjectGridUI::mouseDown(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseDown(e);

	if (e.mods.isLeftButtonDown() && e.mods.isAltDown())
	{
		Array<Object *> objects = InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>();
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipValueRef = o->slideManipParameter->floatValue();
		}
	}
}

void ObjectGridUI::mouseDrag(const MouseEvent& e)
{
	BaseItemMinimalUI::mouseDrag(e);

	if (e.mods.isLeftButtonDown() && e.mods.isAltDown())
	{
		const float pixelRange = 200;
		Array<Object*> objects = InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>();
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(o->slideManipValueRef - e.getDistanceFromDragStartY() / pixelRange);
		}
	}
}

void ObjectGridUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	if (IntensityComponent* ic = c->getParentAs<IntensityComponent>())
	{
		if (c == ic->value) repaint();
	}
}
