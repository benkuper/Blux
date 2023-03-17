/*
  ==============================================================================

    SceneUI.h
    Created: 26 Sep 2020 7:49:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SceneUI :
    public BaseItemUI<Scene>
{
public:
    SceneUI(Scene* scene);
    virtual ~SceneUI();

    std::unique_ptr<TriggerButtonUI> loadUI;
    std::unique_ptr<TriggerButtonUI> directLoadUI;
    std::unique_ptr<FloatSliderUI> loadProgressUI;

    static Image seqImage;
    static Image fxImage;

    Rectangle<float> seqRect;
    Rectangle<float> fxRect;

    void paint(Graphics& g) override;
    void resizedInternalHeader(Rectangle<int>& r) override;
    
    void mouseEnter(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDoubleClick(const MouseEvent& e) override;

    void itemDropped(const DragAndDropTarget::SourceDetails& e) override;

    void showPreview(bool doShow);

    void controllableFeedbackUpdateInternal(Controllable* c) override;
};