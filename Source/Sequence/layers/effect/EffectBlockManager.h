/*
  ==============================================================================

    EffectBlockManager.h
    Created: 10 Oct 2020 10:47:23am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "EffectBlock.h"
class EffectLayer;

class EffectBlockFactory :
    public Factory<LayerBlock>
{
public:
    juce_DeclareSingleton(EffectBlockFactory, true)
    EffectBlockFactory();
};

class EffectBlockManager :
    public LayerBlockManager,
    public EffectBlock::EffectBlockListener
{
public:
    EffectBlockManager(EffectLayer * layer);
    ~EffectBlockManager();

    EffectLayer* effectLayer;

    LayerBlock * createItem() override;

    void addItemInternal(LayerBlock* clip, var) override;
    void removeItemInternal(LayerBlock* clip) override;

    void onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c) override;

    void effectBlockFadesChanged(EffectBlock * block) override;
    void computeFadesForBlock(EffectBlock * block, bool propagate);
};