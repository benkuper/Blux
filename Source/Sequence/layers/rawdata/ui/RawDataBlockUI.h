/*
  ==============================================================================

	RawDataBlockUI.h
	Created: 13 Dec 2022 6:24:21pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once


class RawDataBlockUI :
	public LayerBlockUI,
	public RawDataBlock::AsyncListener
{
public:
	RawDataBlockUI(RawDataBlock* b);
	~RawDataBlockUI();

	RawDataBlock* rawDataBlock;

	void paint(Graphics& g) override;
	void paintOverChildren(Graphics& g) override;

	void resizedBlockInternal() override;

	void newMessage(const RawDataBlock::RawDataBlockEvent& e) override;
};
