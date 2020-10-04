/*
  ==============================================================================

    SceneUI.cpp
    Created: 26 Sep 2020 7:49:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "SceneUI.h"

SceneUI::SceneUI(Scene* scene) :
    BaseItemUI(scene)
{
    loadUI.reset(item->loadTrigger->createButtonUI());
    addAndMakeVisible(loadUI.get());
    loadProgressUI.reset(item->loadProgress->createSlider());
    addAndMakeVisible(loadProgressUI.get());
    loadProgressUI->setVisible(item->defaultLoadTime->floatValue() > 0);
    loadProgressUI->showLabel = false;
    loadProgressUI->showValue = false;
}

SceneUI::~SceneUI()
{
}

void SceneUI::resizedInternalHeader(Rectangle<int>& r)
{
    loadUI->setBounds(r.removeFromRight(60).reduced(2));
    r.removeFromRight(2);
    loadProgressUI->setBounds(r.removeFromRight(100).reduced(2,4));
}

void SceneUI::mouseDown(const MouseEvent& e)
{
    BaseItemUI::mouseDown(e);
    if (e.mods.isLeftButtonDown())
    {
        if (e.mods.isAltDown())
        {
            if (e.mods.isCommandDown()) item->saveScene();
            else item->loadScene();
        }
    }
    else if (e.mods.isRightButtonDown())
    {
        PopupMenu m;
        m.addItem(1, "Save scene");
        m.addItem(2, "Load scene");
        int result = m.show();
        switch (result)
        {
        case 1:
            item->saveScene();
            break;

        case 2:
            item->loadScene();
            break;
        }
    }
}

void SceneUI::controllableFeedbackUpdateInternal(Controllable* c)
{
    BaseItemUI::controllableFeedbackUpdateInternal(c);
    if (c == item->defaultLoadTime)
    {
        loadProgressUI->setVisible(item->defaultLoadTime->floatValue() > 0);
    }
    else if (c == item->loadProgress || c == item->isCurrent)
    {
        bgColor = item->isCurrent->boolValue() ? BLUE_COLOR : (item->loadProgress->floatValue() > 0 ? HIGHLIGHT_COLOR : BG_COLOR.brighter(.1f));
        repaint();
    }
}
