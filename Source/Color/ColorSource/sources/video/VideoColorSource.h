/*
  ==============================================================================

    VideoColorSource.h
    Created: 2 Nov 2020 6:20:48pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class PixelMapColorSource :
    public TimedColorSource
{
public:
    PixelMapColorSource(const String &name = "PixelMap", var params = var());
    virtual ~PixelMapColorSource();

    FloatParameter* brightness;
    FloatParameter* saturation;
    FloatParameter* hue;

    Image sourceImage;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;
};

class VideoColorSource :
    public PixelMapColorSource
{
public:
    VideoColorSource(var params = var());
    ~VideoColorSource();

    FileParameter* file;

    String getTypeString() const override { return "Video File"; }
    static VideoColorSource* create(var params) { return new VideoColorSource(params); }
};

class SpoutSyphonColorSource :
    public PixelMapColorSource
{
public:
    SpoutSyphonColorSource(var params = var());
    ~SpoutSyphonColorSource();

    StringParameter* sourceName;

    String getTypeString() const override { return "Spout/Syphon"; }
    static SpoutSyphonColorSource* create(var params) { return new SpoutSyphonColorSource(params); }
};