/*
  ==============================================================================

    ObjectManagerGridUI.cpp
    Created: 26 Sep 2020 7:51:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectManagerGridUI.h"
#include "Scene/SceneManager.h"

ObjectManagerGridUI::ObjectManagerGridUI(const String& name) :
	BaseManagerShapeShifterUI(name, ObjectManager::getInstance())
{
	contentIsFlexible = true;
	animateItemOnAdd = false;

	thumbSizeUI.reset(ObjectManager::getInstance()->gridThumbSize->createSlider());
	addAndMakeVisible(thumbSizeUI.get());
	thumbSizeUI->useCustomBGColor = true;
	thumbSizeUI->customBGColor = BG_COLOR.darker(.2f);

	flashValueUI.reset(ObjectManager::getInstance()->defaultFlashValue->createSlider());
	addAndMakeVisible(flashValueUI.get());
	flashValueUI->useCustomBGColor = true;
	flashValueUI->customBGColor = BG_COLOR.darker(.2f);

	blackOutUI.reset(ObjectManager::getInstance()->blackOut->createButtonToggle());
	addAndMakeVisible(blackOutUI.get());
	activeInSceneUI.reset(ObjectManager::getInstance()->filterActiveInScene->createButtonToggle());
	addAndMakeVisible(activeInSceneUI.get());

	ObjectManager::getInstance()->addAsyncCoalescedContainerListener(this);

	setShowSearchBar(true);

	addExistingItems();

	SceneManager::getInstance()->addAsyncSceneManagerListener(this);
}

ObjectManagerGridUI::~ObjectManagerGridUI()
{
	if(!inspectable.wasObjectDeleted()) ObjectManager::getInstance()->removeAsyncContainerListener(this);
	if(SceneManager::getInstanceWithoutCreating()) SceneManager::getInstance()->removeAsyncSceneManagerListener(this);
}

void ObjectManagerGridUI::resizedInternalHeader(Rectangle<int>& r)
{
	BaseManagerShapeShifterUI::resizedInternalHeader(r);
	r.removeFromLeft(4);
	activeInSceneUI->setBounds(r.removeFromLeft(100).reduced(3)); 
	r.removeFromLeft(16);
	thumbSizeUI->setBounds(r.removeFromLeft(150).reduced(3));
	r.removeFromLeft(16);
	flashValueUI->setBounds(r.removeFromLeft(150).reduced(3));
	r.removeFromLeft(4);
	blackOutUI->setBounds(r.removeFromLeft(100).reduced(3));
}

void ObjectManagerGridUI::resizedInternalContent(Rectangle<int>& r)
{
	viewport.setBounds(r); 
	
	const int thumbSize = ObjectManager::getInstance()->gridThumbSize->floatValue();

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


void ObjectManagerGridUI::setPreviewData(var data)
{
	for (auto& i : itemsUI)
	{
		i->setPreviewData(data.getProperty(i->item->shortName, var()));
	}
}

bool ObjectManagerGridUI::hasFiltering()
{
	if (BaseManagerUI::hasFiltering()) return true;
	return ObjectManager::getInstance()->filterActiveInScene->boolValue();
}

bool ObjectManagerGridUI::checkFilterForItem(ObjectGridUI* ui)
{
	if (!BaseManagerShapeShifterUI::checkFilterForItem(ui)) return false;


	if (ObjectManager::getInstance()->filterActiveInScene->boolValue() && SceneManager::getInstance()->currentScene != nullptr)
	{
		if (!SceneManager::getInstance()->currentScene->isObjectActiveInScene(ui->item)) return false;
	}

	return true;
	
}


void ObjectManagerGridUI::newMessage(const ContainerAsyncEvent& e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
		if (e.targetControllable == ObjectManager::getInstance()->gridThumbSize || e.targetControllable == ObjectManager::getInstance()->filterActiveInScene) resized();

		break;
        default:
            break;
	}
}
void ObjectManagerGridUI::newMessage(const SceneManager::SceneManagerEvent& e)
{
	if (e.type == e.SCENE_LOAD_START || e.type == e.SCENE_LOAD_END) resized();
}
