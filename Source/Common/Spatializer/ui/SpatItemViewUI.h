/*
  ==============================================================================

    SpatItemViewUI.h
    Created: 24 Dec 2023 10:05:26am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SpatItemViewUI :
	public BaseItemMinimalUI<SpatItem>
{
public:
    SpatItemViewUI(SpatItem* item);
	~SpatItemViewUI();

	void paint(Graphics& g) override;
};