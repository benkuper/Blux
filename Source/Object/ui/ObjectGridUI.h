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
	public BaseItemMinimalUI<Object>,
	public Timer
{
public:
	ObjectGridUI(Object* model);
	~ObjectGridUI();

	bool shouldRepaint;

	Image objectImage;

	std::unique_ptr<IntParameterLabelUI> globalIDUI;
	std::unique_ptr<FloatSliderUI> intensityUI;
	std::unique_ptr<FloatSliderUI> computedIntensityUI;
	std::unique_ptr<ColorParameterUI> computedColorUI;

	void paint(Graphics& g) override;
	void resized() override;

	virtual void updateThumbnail();

	virtual void mouseDown(const MouseEvent& e) override;
	virtual void mouseDrag(const MouseEvent& e) override;

	void controllableFeedbackUpdateInternal(Controllable* c) override;

	void timerCallback() override;
};