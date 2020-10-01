/*
  ==============================================================================

    NoiseEffect.h
    Created: 30 Sep 2020 2:57:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../../Effect.h"
#include "PerlinNoise.hpp"

class NoiseEffect :
    public Effect,
    public HighResolutionTimer
{
public:
    NoiseEffect(var params = var());
    ~NoiseEffect();

    enum NoiseType { SINE, PERLIN };
    EnumParameter * type;
    FloatParameter* amplitude;
    FloatParameter* frequency;
    FloatParameter* offset;
    FloatParameter* offsetByID;

    siv::PerlinNoise perlin;

    double timeAtLastUpdate;
    double curTime;

    var getProcessedComponentValuesInternal(Object * o, ObjectComponent* c, var values) override;

    void onContainerParameterChangedInternal(Parameter* p)override;

    virtual void hiResTimerCallback() override;

    String getTypeString() const override { return "Noise"; }
    static NoiseEffect* create(var params) { return new NoiseEffect(params); }

};