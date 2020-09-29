/*
  ==============================================================================

    ObjectManagerGridUI.cpp
    Created: 26 Sep 2020 7:51:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectManagerGridUI.h"

ObjectManagerGridUI::ObjectManagerGridUI(const String& name) :
	BaseManagerShapeShifterUI(name, ObjectManager::getInstance())
{
	animateItemOnAdd = false;

	thumbSizeUI.reset(manager->gridThumbSize->createSlider());
	addAndMakeVisible(thumbSizeUI.get());

	manager->addAsyncCoalescedContainerListener(this);

	addExistingItems();
}

ObjectManagerGridUI::~ObjectManagerGridUI()
{
	if(!inspectable.wasObjectDeleted()) manager->removeAsyncContainerListener(this);
}

void ObjectManagerGridUI::resizedInternalHeader(Rectangle<int>& r)
{
	BaseManagerShapeShifterUI::resizedInternalHeader(r);
	thumbSizeUI->setBounds(r.removeFromLeft(200).reduced(2));
}

void ObjectManagerGridUI::resizedInternalContent(Rectangle<int>& r)
{
	viewport.setBounds(r); 
	
	const int thumbSize = manager->gridThumbSize->floatValue();

	int numThumbs = itemsUI.size();
	int numThumbPerLine = jmin(r.getWidth() / (thumbSize + gap), numThumbs);
	int numLines = numThumbs == 0 ? 0 : ceil(numThumbs * 1.f / numThumbPerLine);
	
	Rectangle<int> cr;
	cr.setSize(r.getWidth(), numLines * (thumbSize + gap) - gap);
	container.setSize(cr.getWidth(), cr.getHeight());

	int index = 0;
	int yIndex = 0;

	Rectangle<int> lr;

	for (auto& mui : itemsUI)
	{
		if (index % numThumbPerLine == 0)
		{
			int numThumbsInThisLine = jmin(numThumbs - index, numThumbPerLine);
			int lineWidth = numThumbsInThisLine * (thumbSize + gap) - gap;

			if (yIndex > 0) cr.removeFromTop(gap);
			lr = cr.removeFromTop(thumbSize);
			lr = lr.withSizeKeepingCentre(lineWidth, lr.getHeight());

			yIndex++;
		}

		mui->setBounds(lr.removeFromLeft(thumbSize));
		lr.removeFromLeft(gap);
		index++;
	}


}

void ObjectManagerGridUI::newMessage(const ContainerAsyncEvent& e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
		if (e.targetControllable == manager->gridThumbSize) resized();
		break;
	}
}
