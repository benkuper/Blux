/*
  ==============================================================================

    EffectLayer.cpp
    Created: 5 Oct 2020 4:03:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectLayer.h"

EffectLayer::EffectLayer(Sequence* s, var params) :
    SequenceLayer(s, "Effect"),
    blocks(this)
{
}

EffectLayer::~EffectLayer()
{
}
