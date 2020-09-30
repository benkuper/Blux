/*
  ==============================================================================

    GlobalEffectManager.h
    Created: 30 Sep 2020 12:38:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "EffectManager.h"

class EffectGroup :
    public BaseItem
{
public:
    EffectGroup();
    ~EffectGroup();

    String getTypeString() const override { return "EffectGroup"; }
    EffectManager effectManager;
};

class GlobalEffectManager :
    public BaseManager<EffectGroup>
{
public:
    juce_DeclareSingleton(GlobalEffectManager, true);

    GlobalEffectManager();
    ~GlobalEffectManager();

};