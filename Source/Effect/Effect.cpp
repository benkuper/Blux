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
