/*
  ==============================================================================

	RawDataLayerPanel.h
	Created: 13 Dec 2022 6:24:37pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once
class RawDataLayerPanel :
	public SequenceLayerPanel
{
public:
	RawDataLayerPanel(RawDataLayer* layer);
	~RawDataLayerPanel();

	RawDataLayer* rawDataLayer;

	std::unique_ptr<BoolToggleUI> armUI;

	void resizedInternalContent(Rectangle<int>& r) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RawDataLayerPanel)
};