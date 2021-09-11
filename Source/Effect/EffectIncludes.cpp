/*
  ==============================================================================

    EffectIncludes.cpp
    Created: 9 Sep 2021 7:03:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectIncludes.h"

#include "Scene/SceneIncludes.h"
#include "Object/ObjectIncludes.h"

#include "actions/EffectAction.cpp"
#include "Effect.cpp"
#include "EffectManager.cpp"
#include "effects/color/ColorEffect.cpp"
#include "effects/color/gradientmap/ColorGradientMapEffect.cpp"
#include "effects/color/hsv/HSVAdjustEffect.cpp"
#include "effects/color/override/ColorSourceOverrideEffect.cpp"
#include "effects/color/override/ui/ColorSourceOverrideEffectEditor.cpp"
#include "effects/color/remap/ColorPositionRemapEffect.cpp"
#include "effects/override/FreezeEffect.cpp"
#include "effects/override/OverrideEffect.cpp"
#include "effects/time/automation/AutomationEffect.cpp"
#include "effects/time/automation/ui/AutomationEffectEditor.cpp"
#include "effects/time/noise/NoiseEffect.cpp"
#include "effects/time/TimedEffect.cpp"
#include "Filter/Filter.cpp"
#include "Filter/FilterManager.cpp"
#include "Filter/filters/group/GroupFilter.cpp"
#include "Filter/filters/id/IDFilter.cpp"
#include "Filter/filters/layout/LayoutFilter.cpp"
#include "Filter/filters/layout/ui/LayoutFilterStageView.cpp"
#include "Filter/filters/layout/ui/LayoutFilterStageViewUI.cpp"
#include "Filter/ui/FilterEditor.cpp"
#include "Filter/ui/FilterManagerEditor.cpp"
#include "GlobalEffectManager.cpp"
#include "ui/EffectChainVizUI.cpp"
#include "ui/EffectEditor.cpp"
#include "ui/EffectManagerEditor.cpp"
#include "ui/GlobalEffectManagerUI.cpp"