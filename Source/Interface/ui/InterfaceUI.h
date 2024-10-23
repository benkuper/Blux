/*
  ==============================================================================

    InterfaceUI.h
    Created: 26 Sep 2020 7:47:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class InterfaceUI :
    public BaseItemUI<Interface>,
    public Interface::AsyncListener
{
public:
    InterfaceUI(Interface* item);
    virtual ~InterfaceUI();

    std::unique_ptr<TriggerImageUI> inActivityUI;
    std::unique_ptr<TriggerImageUI> outActivityUI;
    std::unique_ptr<BoolToggleUI> connectedUI;

    void paintOverChildren(Graphics& g);
    virtual void resizedHeader(Rectangle<int>& r) override;
    virtual void mouseDown(const MouseEvent &e) override;
    
    void updateConnectedUI();

    void newMessage(const Interface::InterfaceEvent& e) override;

};