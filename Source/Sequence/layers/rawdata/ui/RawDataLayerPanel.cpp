/*
  ==============================================================================

    RawDataLayerPanel.cpp
    Created: 13 Dec 2022 6:24:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataLayerPanel::RawDataLayerPanel(RawDataLayer* layer) :
    SequenceLayerPanel(layer),
    rawDataLayer(layer)
{
    armUI.reset(layer->arm->createToggle());
    addAndMakeVisible(armUI.get());
}

RawDataLayerPanel::~RawDataLayerPanel()
{
}

void RawDataLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
    armUI->setBounds(r.removeFromTop(20));
}
