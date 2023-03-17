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
    Point2DParameter* valueRange;
    std::unique_ptr<siv::PerlinNoise> perlin;

    void processComponentTimeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1, float originalTime = -1) override;

    float getCurrentTime(Object*o, ObjectComponent * c, int id, float timeOverride) override;

    String getTypeString() const override { return getTypeStringStatic(); };
    const static String getTypeStringStatic() { return "Noise"; }

    static NoiseEffect* create(var params) { return new NoiseEffect(params); }

};