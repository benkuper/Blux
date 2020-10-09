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


    addExistingItems();

}

SceneManagerUI::~SceneManagerUI()
{
}

void SceneManagerUI::resizedInternalHeader(Rectangle<int>& r)
{
    Rectangle<int> ar = r.removeFromRight(24);
    BaseManagerShapeShifterUI::resizedInternalHeader(ar);

    prevSceneUI->setBounds(r.removeFromLeft(100).reduced(1));
    r.removeFromLeft(8);
    nextSceneUI->setBounds(r.removeFromLeft(100).reduced(1));
    r.removeFromLeft(8);
    autoPreviewUI->setBounds(r.removeFromLeft(60).reduced(1));
}
