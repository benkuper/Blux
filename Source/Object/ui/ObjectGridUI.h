/*
  ==============================================================================

    ObjectGridUI.h
    Created: 26 Sep 2020 7:51:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
 
#include "../Object.h"

class ObjectGridUI :
	public BaseItemMinimalUI<Object>
{
public:
	ObjectGridUI(Object* model);
	~ObjectGridUI();

	Image objectImage;

	void paint(Graphics& g) override;

	virtual void updateThumbnail();

	virtual void mouseDown(const MouseEvent& e) override;
	virtual void mouseDrag(const MouseEvent& e) override;
};