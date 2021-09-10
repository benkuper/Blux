/*
  ==============================================================================

	GroupUI.cpp
	Created: 29 Sep 2020 9:46:46pm
	Author:  bkupe

  ==============================================================================
*/

GroupUI::GroupUI(Group* item) :
	BaseItemUI(item),
	flashMode(false)
{
	acceptedDropTypes.add("Object");
}

GroupUI::~GroupUI()
{
}

void GroupUI::mouseDown(const MouseEvent& e)
{
	BaseItemUI::mouseDown(e);

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
		BaseItemUI::mouseDrag(e);
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
			for (auto& o : objects) if (o->slideManipParameter != nullptr) actions.add(o->slideManipParameter->setUndoableValue(o->slideManipValueRef, o->slideManipParameter->floatValue(), true));
			UndoMaster::getInstance()->performActions("Change " + String(actions.size()) + " values ", actions);
		}
	}
	else
	{
		BaseItemUI::mouseUp(e);
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

void GroupUI::itemDropped(const DragAndDropTarget::SourceDetails& details)
{
	String dataType = details.description.getProperty("dataType", "");
	if (dataType == "Object")
	{
		if (BaseItemMinimalUI<Object>* bi = static_cast<BaseItemMinimalUI<Object>*>(details.sourceComponent.get()))
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

	BaseItemUI::itemDropped(details);
}
