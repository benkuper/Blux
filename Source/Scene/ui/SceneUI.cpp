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
}

SceneUI::~SceneUI()
{
}

void SceneUI::mouseDown(const MouseEvent& e)
{
    BaseItemUI::mouseDown(e);
    if (e.mods.isAltDown())
    {
        if (e.mods.isCommandDown()) item->saveScene();
        else item->loadScene();
    }
}
