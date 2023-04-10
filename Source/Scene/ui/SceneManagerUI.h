/*
  ==============================================================================

    SceneManagerUI.h
    Created: 26 Sep 2020 7:49:30pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SceneManagerUI :
    public BaseManagerShapeShifterUI<SceneManager, Scene, SceneUI>,
    public AsyncSceneListener
{
public:
    SceneManagerUI(const String &name);
    ~SceneManagerUI();

    std::unique_ptr<TriggerButtonUI> prevSceneUI;
    std::unique_ptr<TriggerButtonUI> nextSceneUI;
    std::unique_ptr<EnumParameterUI> previewModeUI;

    void mouseEnter(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;

    void resizedInternalHeader(Rectangle<int>& r) override;

    void newMessage(const SceneManagerEvent& e) override;
    virtual void newMessage(const InspectableSelectionManager::SelectionEvent& e) override;

    static SceneManagerUI* create(const String& name) { return new SceneManagerUI(name); }

};