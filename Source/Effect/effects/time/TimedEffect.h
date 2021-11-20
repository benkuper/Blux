/*
  ==============================================================================

	TimedEffect.h
	Created: 9 Oct 2020 9:16:14pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class TimedEffect :
	public Effect,
	public HighResolutionTimer,
	public ObjectManager::ManagerListener
{
public:
	TimedEffect(const String& name, var params = var());
	virtual ~TimedEffect();

	FloatParameter* speed;
	FloatParameter* timeOffset;

	FloatParameter* offsetByID;
	FloatParameter* offsetByValue;
	Trigger* resetTimeTrigger;
	BoolParameter* autoResetOnNonZero;

	double timeAtLastUpdate;
	HashMap<ObjectComponent*, float> curTimes;
	HashMap<ObjectComponent*, var> prevValues;


	virtual void onContainerTriggerTriggered(Trigger* t) override;
	virtual void updateEnabled() override;

	var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1) override;
	virtual var getProcessedComponentValueTimeInternal(Object* o, ObjectComponent* c, var value, int id, float time) { return value; }

	virtual float getCurrentTime(ObjectComponent * c, float timeOverride = -1);
	virtual void resetTimes();
	virtual void resetTime(Object* o);

	virtual void itemRemoved(Object* o) override;
	virtual void itemsRemoved(Array<Object*> o) override;


	virtual void hiResTimerCallback() override;
	virtual void addTime();
};