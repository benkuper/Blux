/*
  ==============================================================================

    Effect.cpp
    Created: 30 Sep 2020 12:38:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "Effect.h"

Effect::Effect(const String& name, var params) :
    BaseItem(name)
{
    saveAndLoadRecursiveData = true;

    addChildControllableContainer(&filterManager);
    showInspectorOnSelect = false;
}

Effect::~Effect()
{
}

ThreadedEffect::ThreadedEffect(const String& name, var params) :
    Effect(name, params),
    Thread("Effect "+name)
{
    startThread();
}

ThreadedEffect::~ThreadedEffect()
{
    stopThread(1000);
}

void ThreadedEffect::onContainerParameterChangedInternal(Parameter* p)
{
    Effect::onContainerParameterChangedInternal(p);

    if (p == enabled)
    {
        if (enabled->boolValue()) startThread();
        else stopThread(1000);
    }
}

void ThreadedEffect::run()
{
    while (!threadShouldExit())
    {
        long millisBefore = Time::getMillisecondCounter();
        runInternal();
        long millisAfter = Time::getMillisecondCounter();

        long millisToSleep = jmax<long>(1, fps - (millisAfter - millisBefore));
        sleep(millisToSleep);
    }
}
