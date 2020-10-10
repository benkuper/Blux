/*
  ==============================================================================

    EffectBlockUI.h
    Created: 10 Oct 2020 10:47:39am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../EffectBlock.h"

class EffectBlockFadeHandle :
    public Component
{
public:
    EffectBlockFadeHandle();
    ~EffectBlockFadeHandle() {}

    void paint(Graphics& g) override;
};


class EffectBlockUI :
    public LayerBlockUI,
    public EffectBlock::AsyncListener
{
public:
    EffectBlockUI(EffectBlock * b);
    ~EffectBlockUI();

    EffectBlock* effectBlock;

    float fadeValueAtMouseDown;
    EffectBlockFadeHandle fadeInHandle;
    EffectBlockFadeHandle fadeOutHandle;

    void paint(Graphics& g) override;
    void paintOverChildren(Graphics& g) override;

    void resizedBlockInternal() override;

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void controllableFeedbackUpdateInternal(Controllable*) override;
    void newMessage(const EffectBlock::EffectBlockEvent& e) override;

};
