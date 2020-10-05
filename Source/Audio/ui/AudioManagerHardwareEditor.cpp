/*
  ==============================================================================

    AudioManagerEditor.cpp
    Created: 27 Apr 2018 12:04:23pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioManagerHardwareEditor.h"


AudioManagerHardwareEditor::AudioManagerHardwareEditor(AudioModuleHardwareSettings * hs, bool isRoot) :
	GenericControllableContainerEditor(hs, isRoot),
	hs(hs),
	selector(*hs->am, 0, 16, 0, 16, false, false, false, false)
{
	addAndMakeVisible(selector);
	selector.setSize(100, 500);
	selector.setVisible(!container->editorIsCollapsed);

	setSize(100,600);
}

AudioManagerHardwareEditor::~AudioManagerHardwareEditor()
{
}

void AudioManagerHardwareEditor::setCollapsed(bool value, bool force, bool animate, bool doNotRebuild)
{
	GenericControllableContainerEditor::setCollapsed(value, force, animate, doNotRebuild);
	selector.setVisible(!container->editorIsCollapsed);
}

void AudioManagerHardwareEditor::resizedInternalContent(Rectangle<int>& r)
{
	selector.setBounds(r.withHeight(selector.getHeight()));
	r.setY(selector.getHeight());
	r.setHeight(0);
}
