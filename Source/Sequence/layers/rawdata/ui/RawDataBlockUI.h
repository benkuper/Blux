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

	float fadeValueAtMouseDown;
	EffectBlockFadeHandle fadeInHandle;
	EffectBlockFadeHandle fadeOutHandle;

	void paint(Graphics& g) override;
	void paintOverChildren(Graphics& g) override;

	void resizedBlockInternal() override;

	void mouseDown(const MouseEvent& e) override;
	void mouseDrag(const MouseEvent& e) override;
	void mouseUp(const MouseEvent& e) override;

	void controllableFeedbackUpdateInternal(Controllable*) override;
	void newMessage(const RawDataBlock::RawDataBlockEvent& e) override;
};
