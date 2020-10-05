/*
  ==============================================================================

    EffectLayer.h
    Created: 5 Oct 2020 4:03:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class EffectLayer :
    public SequenceLayer
{
public:
    EffectLayer(Sequence* s, var params = var());
    ~EffectLayer();

    LayerBlockManager blocks;

    String getTypeString() const override { return "Effect"; }
    static EffectLayer* create(Sequence* s, var params) { return new EffectLayer(s, params); }
};