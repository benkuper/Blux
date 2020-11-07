/*
  ==============================================================================

    ColorSource.h
    Created: 2 Nov 2020 6:13:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Object;
class ColorComponent;

class ColorSource :
    public BaseItem
{
public:
    ColorSource(const String &name = "ColorSource", var params = var());
    virtual ~ColorSource();

    String sourceType;
    File imgPath;

    Array<Colour> getColorsForObject(Object* o, ColorComponent * c, int id = -1, float time = -1);
    virtual void fillColorsForObject(Array<Colour>& colors, Object* o, ColorComponent* c, int id = -1, float time = -1);
};

class TimedColorSource :
    public ColorSource,
    public Timer
{
public:
    TimedColorSource(const String& name, var params = var());
    virtual ~TimedColorSource();

    FloatParameter* speed;
    FloatParameter* timeOffset;

    FloatParameter* offsetByID;

    double timeAtLastUpdate;
    double curTime;

    virtual void fillColorsForObject(Array<Colour>& colors, Object* o, ColorComponent* c, int id = -1, float time = -1) override;
    virtual void fillColorsForObjectTimeInternal(Array<Colour>& colors, Object* o, ColorComponent* c, int id, float time) { }

    virtual float getCurrentTime(float timeOverride = -1);

    virtual void timerCallback() override;
    virtual void addTime();

    virtual void onContainerParameterChangedInternal(Parameter* p) override;
};