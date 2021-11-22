/*
  ==============================================================================

    PatternColorSource.h
    Created: 2 Nov 2020 6:20:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SolidColorSource :
    public TimedColorSource
{
public:
    SolidColorSource(var params = var());
    ~SolidColorSource();

    ColorParameter* sourceColor;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;

    String getTypeString() const override { return "Solid Color"; }
    static SolidColorSource* create(var params) { return new SolidColorSource(params); }
};

class RainbowColorSource :
    public TimedColorSource
{
public:
    RainbowColorSource(var params = var());
    ~RainbowColorSource();

    FloatParameter* density;
    FloatParameter* offset;
    FloatParameter* brightness;
    FloatParameter* saturation;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;

    String getTypeString() const override { return "Rainbow"; }
    static RainbowColorSource* create(var params) { return new RainbowColorSource(params); }
};

class StrobeColorSource :
    public TimedColorSource
{
public:
    StrobeColorSource(var params = var());
    ~StrobeColorSource();

    ColorParameter* colorON;
    ColorParameter* colorOFF;
    FloatParameter* onOffBalance;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;

    String getTypeString() const override { return "Strobe"; }
    static StrobeColorSource* create(var params) { return new StrobeColorSource(params); }
};

namespace siv
{
    class PerlinNoise;
}

class NoiseColorSource :
    public TimedColorSource
{
public:
    NoiseColorSource(var params = var());
    ~NoiseColorSource();

    std::unique_ptr<siv::PerlinNoise> perlin;

    FloatParameter* brightness;
    FloatParameter* scale;
    FloatParameter* contrast;
    FloatParameter* balance;
    ColorParameter* frontColor;
    ColorParameter* bgColor;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;
   
    String getTypeString() const override { return "Noise"; }
    static NoiseColorSource* create(var params) { return new NoiseColorSource(params); }
};

class PointColorSource :
    public ColorSource
{
public:
    PointColorSource(var params = var());
    ~PointColorSource();

    FloatParameter* brightness;
    FloatParameter* position;
    FloatParameter* size;
    FloatParameter* fade;
    ColorParameter* pointColor;
    ColorParameter* bgColor;
    IntParameter* extendNum;
    BoolParameter* invertOdds;
    BoolParameter* invertEvens;

    virtual void fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time = -1) override;
   
    String getTypeString() const override { return "Point"; }
    static PointColorSource* create(var params) { return new PointColorSource(params); }
};

class MultiPointColorSource :
    public TimedColorSource
{
public:
    MultiPointColorSource(var params = var());
    ~MultiPointColorSource();

    FloatParameter* brightness;
    FloatParameter* gap;
    FloatParameter* size;
    FloatParameter* fade;
    ColorParameter* pointColor;
    ColorParameter* bgColor;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;
    
    String getTypeString() const override { return "Multipoint"; }
    static MultiPointColorSource* create(var params) { return new MultiPointColorSource(params); }
};

class GradientColorSource :
    public TimedColorSource
{
public:
    GradientColorSource(var params = var());
    ~GradientColorSource();

    FloatParameter* brightness;
    FloatParameter* density;

    std::unique_ptr<GradientColorManager> gradient;
    GradientColorManager* gradientTarget;

    void linkToTemplate(ColorSource* sourceTemplate) override;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;

    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

    String getTypeString() const override { return "Gradient"; }
    static GradientColorSource* create(var params) { return new GradientColorSource(params); }
};