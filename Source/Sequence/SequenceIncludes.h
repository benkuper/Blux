/*
  ==============================================================================

	SequenceIncludes.h
	Created: 9 Sep 2021 7:02:15pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Common/CommonIncludes.h"
#include "ChainViz/ChainViz.h"
#include "Effect/EffectIncludes.h"
#include "Interface/InterfaceIncludes.h"

#include "BluxSequence.h"
#include "BluxSequenceManager.h"
#include "GlobalSequenceManager.h"

#include "actions/SequenceAction.h"

#include "layers/action/ActionLayer.h"
#include "layers/action/ActionTrigger.h"
#include "layers/automation/AutomationLayer.h"
#include "layers/colorsource/ColorSourceLayer.h"

#include "layers/effect/EffectBlock.h"
#include "layers/effect/EffectBlockManager.h"
#include "layers/effect/EffectLayer.h"

#include "layers/effect/ui/EffectBlockUI.h"
#include "layers/effect/ui/EffectBlockManagerUI.h"

#include "layers/effect/ui/EffectLayerTimeline.h"

#include "layers/rawdata/RawDataBlock.h"
#include "layers/rawdata/RawDataBlockManager.h"
#include "layers/rawdata/RawDataLayer.h"

#include "layers/rawdata/ui/RawDataBlockUI.h"
#include "layers/rawdata/ui/RawDataBlockManagerUI.h"
#include "layers/rawdata/ui/RawDataLayerTimeline.h"
#include "layers/rawdata/ui/RawDataLayerPanel.h"

#include "ui/BluxSequenceChainVizUI.h"
#include "ui/BluxSequenceEditor.h"
#include "ui/GlobalSequenceManagerUI.h"
