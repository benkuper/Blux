/*
  ==============================================================================

	GroupUI.cpp
	Created: 29 Sep 2020 9:46:46pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

GroupUI::GroupUI(Group* item) :
	ItemUI(item),
	flashMode(false)
{
	acceptedDropTypes.add("Object");

	weightUI.reset(item->effectManager->globalWeight->createSlider());
	addAndMakeVisible(weightUI.get());
}

GroupUI::~GroupUI()
{
}

void GroupUI::mouseDown(const MouseEvent& e)
{
	ItemUI::mouseDown(e);

	if (e.mods.isAltDown())
	{
		Array<Object*> objects = item->getObjects();
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipValueRef = o->slideManipParameter->floatValue();
			if (e.mods.isRightButtonDown()) if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(ObjectManager::getInstance()->defaultFlashValue->floatValue());
		}
	}
}

void GroupUI::mouseDrag(const MouseEvent& e)
{

	if (e.mods.isLeftButtonDown() && e.mods.isAltDown())
	{
		const float pixelRange = 200;
		Array<Object*> objects = item->getObjects();
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(o->slideManipValueRef - e.getDistanceFromDragStartY() / pixelRange);
		}
	}
	else
	{
		ItemUI::mouseDrag(e);
	}
}

void GroupUI::mouseUp(const MouseEvent& e)
{

	if (e.mods.isAltDown())
	{
		Array<Object*> objects = item->getObjects();

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
	else
	{
		ItemUI::mouseUp(e);
	}
}

bool GroupUI::keyStateChanged(bool isDown)
{
	if (KeyPress::isKeyCurrentlyDown(KeyPress::createFromDescription("f").getKeyCode()))
	{
		flashMode = true;

		Array<Object*> objects = item->getObjects();
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipValueRef = o->slideManipParameter->floatValue();
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(ObjectManager::getInstance()->defaultFlashValue->floatValue());
		}

		return true;
	}
	else if (flashMode)
	{
		Array<Object*> objects = item->getObjects();
		for (auto& o : objects)
		{
			if (o->slideManipParameter != nullptr) o->slideManipParameter->setValue(o->slideManipValueRef);
		}
		flashMode = false;
		return true;
	}
	return false;
}

void GroupUI::resizedInternalHeader(Rectangle<int>& r)
{
	ItemUI::resizedInternalHeader(r);
	r.removeFromRight(2);
	r.removeFromLeft(2);
	weightUI->setBounds(r.removeFromRight(100));
}

void GroupUI::itemDropped(const DragAndDropTarget::SourceDetails& details)
{
	String dataType = details.description.getProperty("dataType", "");
	if (dataType == "Object")
	{
		if (ItemMinimalUI<Object>* bi = static_cast<ItemMinimalUI<Object>*>(details.sourceComponent.get()))
		{
			if (Object* o = dynamic_cast<Object*>(bi->item))
			{

				if (ObjectGroup* og = dynamic_cast<ObjectGroup*>(item))
				{
					Array<Object*> objects = InspectableSelectionManager::activeSelectionManager->getInspectablesAs<Object>();
					objects.addIfNotAlreadyThere(o);
					og->addObjects(objects);
				}
			}
		}

	}
	else
	{
		NLOGWARNING(item->niceName, "Drop not supported for type : " << dataType);
	}

	ItemUI::itemDropped(details);
}
