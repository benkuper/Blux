/*
  ==============================================================================

    EffectLayer.h
    Created: 5 Oct 2020 4:03:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "EffectBlockManager.h"
class Object;
class ObjectComponent;

class EffectLayer :
    public SequenceLayer
{
public:
    EffectLayer(Sequence* s, var params = var());
    ~EffectLayer();

    EffectBlockManager blockManager;
    FilterManager filterManager;

    FloatParameter* timeOffsetByID;
    
    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f);

    SequenceLayerTimeline * getTimelineUI() override;

    String getTypeString() const override { return "Effect"; }
    static EffectLayer* create(Sequence* s, var params) { return new EffectLayer(s, params); }
};