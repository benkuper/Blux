/*
  ==============================================================================

    SceneManagerUI.cpp
    Created: 26 Sep 2020 7:49:30pm
    Author:  bkupe

  ==============================================================================
*/

#include "SceneManagerUI.h"

SceneManagerUI::SceneManagerUI(const String& name) :
    BaseManagerShapeShifterUI(name, SceneManager::getInstance())
{
    headerSize = 30;
    
    nextSceneUI.reset(manager->loadNextSceneTrigger->createButtonUI());
    addAndMakeVisible(nextSceneUI.get());
   
    prevSceneUI.reset(manager->loadPreviousSceneTrigger->createButtonUI());
    addAndMakeVisible(prevSceneUI.get());

    autoPreviewUI.reset(manager->autoPreview->createButtonToggle());
    addAndMakeVisible(autoPreviewUI.get());


    nextSceneUI->addMouseListener(this, false);
    prevSceneUI->addMouseListener(this, false);

    manager->addAsyncSceneManagerListener(this);

    addExistingItems();
}

SceneManagerUI::~SceneManagerUI()
{
    if(!inspectable.wasObjectDeleted()) manager->removeAsyncSceneManagerListener(this);
}

void SceneManagerUI::mouseEnter(const MouseEvent& e)
{
    BaseManagerShapeShifterUI::mouseEnter(e);
    if (manager->autoPreview->boolValue() && (e.eventComponent == nextSceneUI.get() || e.eventComponent == prevSceneUI.get()))
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
    if (manager->autoPreview->boolValue() && (e.eventComponent == nextSceneUI.get() || e.eventComponent == prevSceneUI.get()))
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

    prevSceneUI->setBounds(r.removeFromLeft(100).reduced(1));
    r.removeFromLeft(8);
    nextSceneUI->setBounds(r.removeFromLeft(100).reduced(1));
    r.removeFromLeft(8);
    autoPreviewUI->setBounds(r.removeFromLeft(60).reduced(1));
}

void SceneManagerUI::newMessage(const SceneManager::SceneManagerEvent& e)
{

    if (e.type == SceneManager::SceneManagerEvent::SCENE_LOAD_START)
    {
        if (manager->autoPreview->boolValue() && (nextSceneUI->isMouseOver() || prevSceneUI->isMouseOver()))
        {
            if (SceneUI* ui = getUIForItem(manager->currentScene)) ui->showPreview(true);
        }

    }else if (e.type == SceneManager::SceneManagerEvent::SCENE_LOAD_END)
    {
        if (manager->autoPreview->boolValue() && (nextSceneUI->isMouseOver() || prevSceneUI->isMouseOver()))
        {
            if (SceneUI* ui = getUIForItem(nextSceneUI->isMouseOver()? manager->getNextScene() : manager->getPreviousScene()))
            {
                ui->showPreview(true);
            }
        }
    }
}
