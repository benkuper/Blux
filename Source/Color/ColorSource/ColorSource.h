/*
  ==============================================================================

    ColorSource.h
    Created: 2 Nov 2020 6:13:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

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

    void fillColorsForObject(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id = -1, float time = -1);
    virtual void fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1);

    virtual void controllableFeedbackUpdate(ControllableContainer * cc, Controllable *c) override;

    virtual void inspectableDestroyed(Inspectable * i) override;
    
    //ui
    virtual String getSourceLabel() const;
};

class TimedColorSource :
    public ColorSource,
    public HighResolutionTimer
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

    virtual void fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1) override;
    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time) { }

    virtual float getCurrentTime(float timeOverride = -1);

    virtual void hiResTimerCallback() override;
    virtual void addTime();

};