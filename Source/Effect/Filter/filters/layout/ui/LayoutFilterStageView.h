/*
  ==============================================================================

    LayoutFilterStageView.h
    Created: 7 Oct 2020 9:09:00am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class LayoutFilterStageView :
    public Component,
    public ContainerAsyncListener,
    public LayoutFilterStageViewUI::AsyncListener,
    public ChangeListener,
    public Timer
{
public:
    LayoutFilterStageView();
    ~LayoutFilterStageView();

    bool shouldRepaint;

    OwnedArray<LayoutFilterStageViewUI> filterUIs;

    Rectangle<float> boundsInView;
    void setBoundsInView(Rectangle<float> r);

    void addUIForFilter(LayoutFilter* f);
    void removeUIForFilter(LayoutFilter* f);
    void removeFilterUI(LayoutFilterStageViewUI* ui);
    LayoutFilterStageViewUI* getUIForFilter(LayoutFilter* f);

    void paint(Graphics& g) override;
    void placeItems();
    void placeItem(LayoutFilterStageViewUI* ui);

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;


    bool hitTest(int x, int y) override;

    void newMessage(const ContainerAsyncEvent& e) override;
    void newMessage(const LayoutFilterStageViewUI::FilterEvent& e) override;

    virtual void changeListenerCallback(ChangeBroadcaster* source) override;

    void timerCallback() override;
};