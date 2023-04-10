/*
  ==============================================================================

    EffectIncludes.h
    Created: 9 Sep 2021 7:03:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

//class ObjectComponent;
//class Object;
//class Group;

class FilterManager;


#include "Object/ObjectIncludes.h"
#include "Common/CommonIncludes.h"
#include "Color/ColorIncludes.h"
#include "ChainViz/ChainViz.h"

#include "actions/EffectAction.h"

#include "Effect.h"
#include "EffectManager.h"

#include "ui/EffectChainVizUI.h"
#include "ui/EffectEditor.h"
#include "ui/EffectManagerEditor.h"

#include "GlobalEffectManager.h"
#include "ui/GlobalEffectManagerUI.h"

#include "effects/color/ColorEffect.h"
#include "effects/color/gradientmap/ColorGradientMapEffect.h"
#include "effects/color/hsv/HSVAdjustEffect.h"
#include "effects/color/override/ColorSourceOverrideEffect.h"
#include "effects/color/override/ui/ColorSourceOverrideEffectEditor.h"
#include "effects/color/remap/ColorPositionRemapEffect.h"

#include "effects/override/OverrideEffect.h"
#include "effects/override/FreezeEffect.h"
#include "effects/override/CurveMapEffect.h"

#include "effects/smooth/SmoothingEffect.h"
#include "effects/smooth/DampingEffect.h"

#include "effects/time/TimedEffect.h"
#include "effects/time/automation/AutomationEffect.h"
#include "effects/time/automation/ui/AutomationEffectEditor.h"
#include "effects/time/noise/NoiseEffect.h"

#include "effects/orientation/OrientationEffect.h"

#include "effects/customcomponent/CustomComponentEffect.h"

#include "Filter/Filter.h"
#include "Filter/FilterManager.h"
#include "Filter/filters/group/GroupFilter.h"
#include "Filter/filters/id/IDFilter.h"
#include "Filter/filters/layout/LayoutFilter.h"

#include "Filter/filters/layout/ui/LayoutFilterStageViewUI.h"
#include "Filter/filters/layout/ui/LayoutFilterStageView.h"

#include "Filter/ui/FilterEditor.h"
#include "Filter/ui/FilterManagerEditor.h"

