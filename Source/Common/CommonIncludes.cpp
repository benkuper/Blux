/*
  ==============================================================================

    CommonIncludes.cpp
    Created: 9 Sep 2021 7:03:49pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommonIncludes.h"
#include "MainIncludes.h"
#include "Scene/SceneIncludes.h"
#include "Object/ObjectIncludes.h"
#include "Sequence/SequenceIncludes.h"
#include "Effect/EffectIncludes.h"


#include "ParameterLink/ParameterLink.cpp"
#include "ParameterLink/ui/LinkableParameterEditor.cpp"

#include "Action/Action.cpp"
#include "Action/ActionManager.cpp"

#include "Helpers/SceneHelpers.cpp"
#include "Helpers/TimedEffectHiresTimer.cpp"
#include "Helpers/ColorHelpers.cpp"

#include "MIDI/MIDIDevice.cpp"
#include "MIDI/MIDIDeviceParameter.cpp"
#include "MIDI/MIDIManager.cpp"
#include "MIDI/MTCReceiver.cpp"
#include "MIDI/MTCSender.cpp"
#include "MIDI/ui/MIDIDeviceChooser.cpp"
#include "MIDI/ui/MIDIDeviceParameterUI.cpp"



#include "Zeroconf/ZeroconfManager.cpp"