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
    public BaseItem,
    public Inspectable::InspectableListener
{
public:
    ColorSource(const String &name = "ColorSource", var params = var());
    virtual ~ColorSource();

    String sourceType;
    File imgPath;

    ColorSource* sourceTemplate;
    WeakReference<Inspectable> sourceTemplateRef;

    virtual void linkToTemplate(ColorSource* st);


    virtual void fillColorsForObject(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id = -1, float time = -1);

    virtual void controllableFeedbackUpdate(ControllableContainer * cc, Controllable *c) override;

    virtual void inspectableDestroyed(Inspectable * i) override;
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

    void linkToTemplate(ColorSource* st) override;

    virtual void fillColorsForObject(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id = -1, float time = -1) override;
    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time) { }

    virtual float getCurrentTime(float timeOverride = -1);

    virtual void timerCallback() override;
    virtual void addTime();

};