/*
  ==============================================================================

    GlobalEffectManagerUI.h
    Created: 30 Sep 2020 1:37:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../GlobalEffectManager.h"

class GlobalEffectManagerUI :
    public BaseManagerShapeShifterUI<GlobalEffectManager, EffectGroup, BaseItemUI<EffectGroup>>
{
public:
    GlobalEffectManagerUI(const String& name);
    ~GlobalEffectManagerUI();

    static GlobalEffectManagerUI* create(const String& name) { return new GlobalEffectManagerUI(name); }
};