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

#define GetSourceLinkedValue(p) sourceParams.getLinkedValue(p, o, id, originalTime)
#define GetLinkedColor(p) getLinkedColor(p, o, id, originalTime)

class ColorSource :
	public BaseItem,
	public Inspectable::InspectableListener,
	public ParamLinkContainer::ParamLinkContainerListener
{
public:
	ColorSource(const String& name = "ColorSource", var params = var());
	virtual ~ColorSource();

	String sourceType;
	File imgPath;

	ParamLinkContainer sourceParams;

	ColorSource* sourceTemplate;
	WeakReference<Inspectable> sourceTemplateRef;

	virtual void linkToTemplate(ColorSource* st);

	virtual void paramControlModeChanged(ParamLinkContainer* pc, ParameterLink* pl) override;

	void fillColorsForObject(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id = -1, float time = -1);
	virtual void fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1);

	virtual void controllableFeedbackUpdate(ControllableContainer* cc, Controllable* c) override;

	virtual void inspectableDestroyed(Inspectable* i) override;

	Colour getLinkedColor(ColorParameter* p, Object* o, int id, float time);

	virtual ColorParameter* getMainColorParameter() { return nullptr; }


	class  ColorSourceListener
	{
	public:
		virtual ~ColorSourceListener() {}
		virtual void colorSourceParamControlModeChanged(Parameter* p) {}
		//virtual void blockBakingProgress(float progress) {}
		//virtual void colorsUpdated() {}
	};

	ListenerList<ColorSourceListener> colorSourceListeners;
	void addColorSourceListener(ColorSourceListener* newListener) { colorSourceListeners.add(newListener); }
	void removeColorSourceListener(ColorSourceListener* listener) { colorSourceListeners.remove(listener); }

	//ui
	virtual String getSourceLabel() const;

};



class TimedColorSource :
	public ColorSource,
	public TimedEffectHiResTimer::TimerListener
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
	virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time, float originalTime) { }

	virtual float getCurrentTime(float timeOverride = -1);

	virtual void addTime();

	virtual void hiResTimerCallback() override;

};

