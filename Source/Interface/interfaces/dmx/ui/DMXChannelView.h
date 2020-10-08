/*
  ==============================================================================

    DMXChannelView.h
    Created: 8 Oct 2020 6:59:51pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Interface/InterfaceManager.h"

class DMXInterface;
class DMXChannelView;

class DMXChannelItem :
    public Component
{
public:
    DMXChannelItem(int channel, DMXChannelView * v);
    ~DMXChannelItem();
    
    int channel;
    float valueAtMouseDown;
    float value;

    DMXChannelView* channelView;

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void updateDMXValue(float val);

    void paint(Graphics& g) override;
};

class DMXChannelView :
    public ShapeShifterContentComponent,
    public InterfaceManager::AsyncListener,
    public ComboBox::Listener,
    public Inspectable::InspectableListener
{
public:

    DMXChannelView(const String &name);
    ~DMXChannelView();

    Viewport viewport;
    Component channelContainer;

    std::unique_ptr<BoolButtonToggleUI> testingUI;

    ComboBox dmxList;
    OwnedArray<DMXChannelItem> channelItems;

    DMXInterface* currentInterface;

    void resized() override;

    void setCurrentInterface(DMXInterface* i);

    void rebuildDMXList();
    void rebuildChannelItems();

    void sendDMXValue(int channel, float value);

    void newMessage(const InterfaceManager::ManagerEvent& e) override;

    // Inherited via Listener
    virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    void inspectableDestroyed(Inspectable* i) override;

    static DMXChannelView* create(const String& name) { return new DMXChannelView(name); }

};