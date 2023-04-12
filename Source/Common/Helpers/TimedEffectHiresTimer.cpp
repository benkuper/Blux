/*
  ==============================================================================

	TimedEffectHiresTimer.cpp
	Created: 12 Apr 2023 1:09:44pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

juce_ImplementSingleton(TimedEffectHiResTimer);

TimedEffectHiResTimer::TimedEffectHiResTimer()
{
	startTimer(50);
}

void TimedEffectHiResTimer::hiResTimerCallback()
{
	timerListeners.call(&TimerListener::hiResTimerCallback);
}
