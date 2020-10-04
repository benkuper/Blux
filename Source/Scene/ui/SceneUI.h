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

    std::unique_ptr<TriggerButtonUI> loadUI;
    std::unique_ptr<FloatSliderUI> loadProgressUI;

    void resizedInternalHeader(Rectangle<int>& r) override;

    void mouseDown(const MouseEvent& e) override;

    void controllableFeedbackUpdateInternal(Controllable* c) override;
};