/*
  ==============================================================================

	SceneManagerUI.cpp
	Created: 26 Sep 2020 7:49:30pm
	Author:  bkupe

  ==============================================================================
*/

#include "Scene/SceneIncludes.h"
#include "SceneManagerUI.h"

SceneManagerUI::SceneManagerUI(const String& name) :
	BaseManagerShapeShifterUI(name, SceneManager::getInstance())
{
	animateItemOnAdd = false;

	headerSize = 30;

	nextSceneUI.reset(manager->loadNextSceneTrigger->createButtonUI());
	addAndMakeVisible(nextSceneUI.get());
	nextSceneUI->customLabel = "Next";

	prevSceneUI.reset(manager->loadPreviousSceneTrigger->createButtonUI());
	addAndMakeVisible(prevSceneUI.get());
	prevSceneUI->customLabel = "Previous";

	previewModeUI.reset(manager->previewMode->createUI());
	addAndMakeVisible(previewModeUI.get());

	nextSceneUI->addMouseListener(this, false);
	prevSceneUI->addMouseListener(this, false);

	manager->addAsyncSceneManagerListener(this);

	setShowSearchBar(true);

	InspectableSelectionManager::mainSelectionManager->addAsyncSelectionManagerListener(this);

	addExistingItems();
}

SceneManagerUI::~SceneManagerUI()
{
	if (!inspectable.wasObjectDeleted()) manager->removeAsyncSceneManagerListener(this);
	if (InspectableSelectionManager::mainSelectionManager != nullptr) InspectableSelectionManager::mainSelectionManager->removeAsyncSelectionManagerListener(this);
}

void SceneManagerUI::mouseEnter(const MouseEvent& e)
{
	BaseManagerShapeShifterUI::mouseEnter(e);
	SceneManager::PreviewMode m = manager->previewMode->getValueDataAsEnum<SceneManager::PreviewMode>();
	if (m == SceneManager::HOVER)
	{
		if (SceneUI* ui = getUIForItem(e.eventComponent == nextSceneUI.get() ? manager->getNextScene() : manager->getPreviousScene()))
		{
			ui->showPreview(true);
		}

	}
}

void SceneManagerUI::mouseExit(const MouseEvent& e)
{
	BaseManagerShapeShifterUI::mouseExit(e);
	SceneManager::PreviewMode m = manager->previewMode->getValueDataAsEnum<SceneManager::PreviewMode>();
	if (m == SceneManager::HOVER)
	{
		if (SceneUI* ui = getUIForItem(e.eventComponent == nextSceneUI.get() ? manager->getNextScene() : manager->getPreviousScene()))
		{
			ui->showPreview(false);
		}
	}
}

void SceneManagerUI::resizedInternalHeader(Rectangle<int>& r)
{
	BaseManagerShapeShifterUI::resizedInternalHeader(r);

	Rectangle<int> pr = r.removeFromLeft(80);
	prevSceneUI->setBounds(pr.removeFromTop(pr.getHeight() / 2).reduced(1));
	nextSceneUI->setBounds(pr.reduced(1));
	r.removeFromLeft(2);
	previewModeUI->setBounds(r.removeFromLeft(100).withHeight(24).reduced(1));
}

void SceneManagerUI::newMessage(const SceneManagerEvent& e)
{
	if (e.type == SceneManagerEvent::SCENE_LOAD_END)
	{
		SceneManager::PreviewMode m = manager->previewMode->getValueDataAsEnum<SceneManager::PreviewMode>();
		if (m == SceneManager::NEXT)
		{
			if (SceneUI* ui = getUIForItem(manager->getNextScene())) ui->showPreview(true);
		}
	}
}

void SceneManagerUI::newMessage(const InspectableSelectionManager::SelectionEvent& e)
{
	BaseManagerUI::newMessage(e);

	if (e.type == InspectableSelectionManager::SelectionEvent::SELECTION_CHANGED)
	{
		SceneManager::PreviewMode m = manager->previewMode->getValueDataAsEnum<SceneManager::PreviewMode>();
		if (m == SceneManager::SELECTED)
		{
			if (SceneUI* ui = getUIForItem(e.selectionManager->getInspectableAs<Scene>())) ui->showPreview(true);
		}
	}
}
