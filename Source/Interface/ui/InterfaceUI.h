/*
  ==============================================================================

    InterfaceUI.h
    Created: 26 Sep 2020 7:47:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class InterfaceUI :
    public BaseItemUI<Interface>
{
public:
    InterfaceUI(Interface* item);
    virtual ~InterfaceUI();

    std::unique_ptr<TriggerImageUI> inActivityUI;
    std::unique_ptr<TriggerImageUI> outActivityUI;

    void paintOverChildren(Graphics& g);
    virtual void resizedHeader(Rectangle<int>& r) override;
    virtual void mouseDown(const MouseEvent &e) override;

};