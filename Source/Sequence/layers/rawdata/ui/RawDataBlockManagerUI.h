/*
  ==============================================================================

    RawDataBlockManagerUI.h
    Created: 13 Dec 2022 6:24:30pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class RawDataLayerTimeline;

class RawDataBlockManagerUI :
    public LayerBlockManagerUI
{
public:
    RawDataBlockManagerUI(RawDataLayerTimeline* timeline);
    ~RawDataBlockManagerUI();

    RawDataLayerTimeline* effectTimeline;

    void mouseDoubleClick(const MouseEvent& e) override;

    LayerBlockUI* createUIForItem(LayerBlock* b) override;
};