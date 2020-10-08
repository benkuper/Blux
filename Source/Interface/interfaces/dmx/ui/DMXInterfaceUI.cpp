/*
  ==============================================================================

    DMXInterfaceUI.cpp
    Created: 8 Oct 2020 6:59:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "DMXInterfaceUI.h"

DMXInterfaceUI::DMXInterfaceUI(DMXInterface* i) :
    InterfaceUI(i),
    dmxInterface(i)
{
}

DMXInterfaceUI::~DMXInterfaceUI()
{
}

void DMXInterfaceUI::controllableFeedbackUpdateInternal(Controllable* c)
{
    InterfaceUI::controllableFeedbackUpdateInternal(c);

    if (c == dmxInterface->channelTestingMode)
    {
        bgColor = dmxInterface->channelTestingMode->boolValue() ? YELLOW_COLOR : (BG_COLOR.brighter(.1f));
        repaint();
    }
}
