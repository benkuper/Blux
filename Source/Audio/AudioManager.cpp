/*
  ==============================================================================

	AudioManager.cpp
	Created: 27 Apr 2018 12:03:27pm
	Author:  Ben

  ==============================================================================
*/

juce_ImplementSingleton(AudioManager)

AudioManager::AudioManager() :
	ControllableContainer("Audio Manager"),
	hs(&am)
{
	
	am.initialiseWithDefaultDevices(2, 2);
	am.addAudioCallback(&player);

	graph.reset();

	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);

	graph.setPlayConfigDetails(0, 2, setup.sampleRate, setup.bufferSize);
	graph.prepareToPlay(setup.sampleRate, setup.bufferSize);

	//graph.addNode(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode), 1);

	std::unique_ptr<AudioProcessorGraph::AudioGraphIOProcessor> proc(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
	graph.addNode(std::move(proc), AudioProcessorGraph::NodeID(2));

	player.setProcessor(&graph);

	addChildControllableContainer(&hs);
}


AudioManager::~AudioManager()
{

	DBG("Remove audio callback here");
	am.removeAudioCallback(&player);

	graph.clear();
	player.setProcessor(nullptr);
}

var AudioManager::getJSONData(bool includeNonOverriden)
{
	var data = ControllableContainer::getJSONData(includeNonOverriden);

	std::unique_ptr<XmlElement> xmlData(am.createStateXml());
	if (xmlData != nullptr) data.getDynamicObject()->setProperty("audioSettings", xmlData->toString());

	return data;
}

void AudioManager::loadJSONDataInternal(var data)
{
	ControllableContainer::loadJSONDataInternal(data);

	if (data.getDynamicObject()->hasProperty("audioSettings"))
	{

		std::unique_ptr<XmlElement> elem = XmlDocument::parse(data.getProperty("audioSettings", ""));
		am.initialise(2, 2, elem.get(), true);
	}
}

AudioModuleHardwareSettings::AudioModuleHardwareSettings(AudioDeviceManager * am) :
	ControllableContainer("Hardware Settings"),
	am(am)
{
	hideEditorHeader = true;
	
}

InspectableEditor * AudioModuleHardwareSettings::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new AudioManagerHardwareEditor(this, isRoot);
}
