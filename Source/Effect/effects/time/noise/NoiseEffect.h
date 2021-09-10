/*
  ==============================================================================

    NoiseEffect.h
    Created: 30 Sep 2020 2:57:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

namespace siv
{
    class PerlinNoise;
}

class NoiseEffect :
    public TimedEffect
{
public:
    NoiseEffect(var params = var());
    ~NoiseEffect();

    enum NoiseType { SINE, PERLIN };
    EnumParameter * type;
    FloatParameter* frequency;
    FloatParameter* amplitude;
    FloatParameter* valueOffset;
    std::unique_ptr<siv::PerlinNoise> perlin;

    var getProcessedComponentValueTimeInternal(Object * o, ObjectComponent* c, var value, int id, float time) override;

    float getCurrentTime(float timeOverride) override;

    String getTypeString() const override { return getTypeStringStatic(); };
    const static String getTypeStringStatic() { return "Noise"; }

    static NoiseEffect* create(var params) { return new NoiseEffect(params); }

};