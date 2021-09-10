/*
  ==============================================================================

    EffectBlockManagerUI.h
    Created: 10 Oct 2020 10:49:03am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class EffectLayerTimeline;

class EffectBlockManagerUI :
    public LayerBlockManagerUI
{
public:
    EffectBlockManagerUI(EffectLayerTimeline * timeline);
    ~EffectBlockManagerUI();

    EffectLayerTimeline* effectTimeline;

    LayerBlockUI* createUIForItem(LayerBlock* b) override;

    void showMenuAndAddItem(bool fromAddButton, Point<int> mouseDownPos) override;
    void mouseDoubleClick(const MouseEvent &e) override;
};