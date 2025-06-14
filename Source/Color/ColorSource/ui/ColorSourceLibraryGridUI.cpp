/*
  ==============================================================================

    ColorSourceLibraryGridUI.cpp
    Created: 3 Nov 2020 11:21:01am
    Author:  bkupe

  ==============================================================================
*/

ColorSourceLibraryGridUI::ColorSourceLibraryGridUI(const String& name) :
    ManagerShapeShifterUI(name, ColorSourceLibrary::getInstance())
{
	thumbSizeUI.reset(manager->gridThumbSize->createSlider());
	addAndMakeVisible(thumbSizeUI.get());
	thumbSizeUI->useCustomBGColor = true;
	thumbSizeUI->customBGColor = BG_COLOR.darker(.2f);
	manager->addAsyncCoalescedContainerListener(this);

	addExistingItems();
}

ColorSourceLibraryGridUI::~ColorSourceLibraryGridUI()
{
	if(!inspectable.wasObjectDeleted()) manager->removeAsyncContainerListener(this);
}

void ColorSourceLibraryGridUI::resizedInternalHeader(Rectangle<int>& r)
{
	ManagerShapeShifterUI::resizedInternalHeader(r);
	thumbSizeUI->setBounds(r.removeFromLeft(150).reduced(3));
}

void ColorSourceLibraryGridUI::resizedInternalContent(Rectangle<int>& r)
{
	viewport.setBounds(r);

	const int thumbSize = manager->gridThumbSize->floatValue();

	int numThumbs = getFilteredItems().size();
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
		if (!checkFilterForItem(mui))
		{
			mui->setVisible(false);
			continue;
		}

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

void ColorSourceLibraryGridUI::newMessage(const ContainerAsyncEvent& e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
		if (e.targetControllable == manager->gridThumbSize) resized();

		break;
	default:
		break;
	}
}
