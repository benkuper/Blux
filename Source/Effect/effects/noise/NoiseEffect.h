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
    public Effect
{
public:
    NoiseEffect(var params = var());
    ~NoiseEffect();

    enum NoiseType { SINE, PERLIN };
    EnumParameter * type;
    FloatParameter* amplitude;
    FloatParameter* frequency;

    siv::PerlinNoise perlin;

    void processComponentValues(Object * o, ObjectComponent* c, var &values) override;

    String getTypeString() const override { return "Noise"; }
    static NoiseEffect* create(var params) { return new NoiseEffect(params); }
};