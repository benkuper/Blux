/*
  ==============================================================================

    AutomationLayer.cpp
    Created: 5 Oct 2020 4:03:08pm
    Author:  bkupe

  ==============================================================================
*/

AutomationLayer::AutomationLayer(Sequence* s, var params) :
    SequenceLayer(s, "Automation"),
    blocks(this)
{
    addChildControllableContainer(&blocks);
}

AutomationLayer::~AutomationLayer()
{
}
