/*
  ==============================================================================

    GroupUI.h
    Created: 29 Sep 2020 9:46:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class GroupUI :
    public ItemUI<Group>
{
public:
    GroupUI(Group * item);
    ~GroupUI();


    bool flashMode;
    std::unique_ptr<FloatSliderUI> weightUI;


    virtual void mouseDown(const MouseEvent& e) override;
    virtual void mouseDrag(const MouseEvent& e) override;
    virtual void mouseUp(const MouseEvent& e) override;
    
    virtual bool keyStateChanged(bool isDown) override;

    virtual void resizedInternalHeader(Rectangle<int>& r) override;

    void itemDropped(const DragAndDropTarget::SourceDetails& details) override;
};