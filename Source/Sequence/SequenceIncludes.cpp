/*
  ==============================================================================

    SequenceIncludes.cpp
    Created: 9 Sep 2021 7:02:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceIncludes.h"
#include "Object/ObjectIncludes.h"
#include "Effect/EffectIncludes.h"
#include "MainIncludes.h"


#include "actions/SequenceAction.cpp"

#include "BluxSequence.cpp"
#include "BluxSequenceManager.cpp"

#include "GlobalSequenceManager.cpp"

#include "layers/action/ActionLayer.cpp"
#include "layers/action/ActionTrigger.cpp"

#include "layers/automation/AutomationLayer.cpp"

#include "layers/colorsource/ColorSourceLayer.cpp"

#include "layers/effect/EffectBlock.cpp"
#include "layers/effect/EffectBlockManager.cpp"
#include "layers/effect/EffectLayer.cpp"
#include "layers/effect/ui/EffectBlockManagerUI.cpp"
#include "layers/effect/ui/EffectBlockUI.cpp"
#include "layers/effect/ui/EffectLayerTimeline.cpp"

#include "ui/BluxSequenceChainVizUI.cpp"
#include "ui/BluxSequenceEditor.cpp"
#include "ui/GlobalSequenceManagerUI.cpp"