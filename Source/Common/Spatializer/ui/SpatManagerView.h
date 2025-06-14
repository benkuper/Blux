/*
  ==============================================================================

	SpatManagerView.h
	Created: 24 Dec 2023 10:05:15am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class SpatManagerView :
	public ManagerViewUI<SpatManager, SpatItem, SpatItemViewUI>
{
public:
	SpatManagerView(SpatManager* manager);
	~SpatManagerView();

};