/*
  ==============================================================================

    AutomationLayer.cpp
    Created: 5 Oct 2020 4:03:08pm
    Author:  bkupe

  ==============================================================================
*/

#include "AutomationLayer.h"

AutomationLayer::AutomationLayer(Sequence* s, var params) :
    SequenceLayer(s, "Automation"),
    blocks(this)
{
    addChildControllableContainer(&blocks);
}

AutomationLayer::~AutomationLayer()
{
}
