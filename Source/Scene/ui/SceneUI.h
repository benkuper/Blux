/*
  ==============================================================================

    SceneUI.h
    Created: 26 Sep 2020 7:49:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Scene.h"

class SceneUI :
    public BaseItemUI<Scene>
{
public:
    SceneUI(Scene* scene);
    virtual ~SceneUI();

    void mouseDown(const MouseEvent& e) override;
};