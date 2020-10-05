/*
  ==============================================================================

    AudioManager.h
    Created: 27 Apr 2018 12:03:27pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#define AUDIO_INPUT_GRAPH_ID 1
#define AUDIO_OUTPUT_GRAPH_ID 2

class AudioModuleHardwareSettings :
	public ControllableContainer
{
public:
	AudioModuleHardwareSettings(AudioDeviceManager * am);
	~AudioModuleHardwareSettings() {}
	AudioDeviceManager * am;

	InspectableEditor * getEditor(bool isRoot) override;
};

class AudioManager :
	public ControllableContainer
{
public:
	juce_DeclareSingleton(AudioManager, true);

	AudioManager();
	~AudioManager();

	AudioModuleHardwareSettings hs;
	AudioDeviceManager am;
	AudioProcessorPlayer player;
	AudioProcessorGraph graph;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
};
