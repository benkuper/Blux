/*
  ==============================================================================

	TimedEffectHiresTimer.h
	Created: 12 Apr 2023 1:09:44pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class TimedEffectHiResTimer :
	public HighResolutionTimer
{
public:
	juce_DeclareSingleton(TimedEffectHiResTimer, true);

	TimedEffectHiResTimer();
	~TimedEffectHiResTimer() {}

	virtual void hiResTimerCallback() override;

	class TimerListener
	{
	public:
		/** Destructor. */
		virtual ~TimerListener() {}
		virtual void hiResTimerCallback() {}
	};

	ListenerList<TimerListener> timerListeners;
	void addTimerListener(TimerListener* newListener) { timerListeners.add(newListener); }
	void removeTimerListener(TimerListener* listener) { timerListeners.remove(listener); }
};