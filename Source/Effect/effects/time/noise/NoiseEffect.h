/*
  ==============================================================================

    NoiseEffect.h
    Created: 30 Sep 2020 2:57:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../TimedEffect.h"
#include "PerlinNoise.hpp"

class NoiseEffect :
    public TimedEffect
{
public:
    NoiseEffect(var params = var());
    ~NoiseEffect();

    enum NoiseType { SINE, PERLIN };
    EnumParameter * type;
    FloatParameter* amplitude;
    siv::PerlinNoise perlin;

    var getProcessedComponentValueTimeInternal(Object * o, ObjectComponent* c, int id, var value, float time) override;

    String getTypeString() const override { return "Noise"; }
    static NoiseEffect* create(var params) { return new NoiseEffect(params); }

};