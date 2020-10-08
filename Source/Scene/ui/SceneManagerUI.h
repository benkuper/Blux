/*
  ==============================================================================

    SceneManagerUI.h
    Created: 26 Sep 2020 7:49:30pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../SceneManager.h"
#include "SceneUI.h"

class SceneManagerUI :
    public BaseManagerShapeShifterUI<SceneManager, Scene, SceneUI>
{
public:
    SceneManagerUI(const String &name);
    ~SceneManagerUI();

    std::unique_ptr<TriggerButtonUI> prevSceneUI;
    std::unique_ptr<TriggerButtonUI> nextSceneUI;


    void resizedInternalHeader(Rectangle<int>& r) override;

    static SceneManagerUI* create(const String& name) { return new SceneManagerUI(name); }

};