/*
  ==============================================================================

    ObjectGridUI.h
    Created: 26 Sep 2020 7:51:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
 
class ColorViz;

class ObjectGridUI :
	public BaseItemMinimalUI<Object>,
	public ComponentManager::AsyncListener,
	public ColorComponent::AsyncColorComponentListener
{
public:
	ObjectGridUI(Object* model);
	~ObjectGridUI();

	bool shouldRepaint;
	bool transparentBG;

	//preview
	var previewData;
	float previewIntensity;

	FloatParameter* iconIntensityRef;
	Image objectImage;
	Image objectONImage;

	std::unique_ptr<IntParameterLabelUI> globalIDUI;
	std::unique_ptr<FloatSliderUI> intensityUI;
	std::unique_ptr<FloatSliderUI> computedIntensityUI;
	std::unique_ptr<ColorParameterUI> mainColorUI;
	std::unique_ptr<ColorViz> colorViz;

	bool flashMode;

	void paint(Graphics& g) override;
	void paintOverChildren(Graphics& g) override;
	void resized() override;

	virtual void updateUI();
	virtual void updateThumbnail();

	void setPreviewData(var data = var());

	virtual void mouseDown(const MouseEvent& e) override;
	virtual void mouseDrag(const MouseEvent& e) override;
	virtual void mouseUp(const MouseEvent& e) override;
	virtual void mouseDoubleClick(const MouseEvent& e) override;

	virtual bool keyStateChanged(bool isDown) override;

	void controllableFeedbackUpdateInternal(Controllable* c) override;

	void visibilityChanged() override;

	void newMessage(const ComponentManager::ManagerEvent& e) override;
	void newMessage(const ColorComponent::ColorComponentEvent& e) override;

	void handleRepaint();
};

class ObjectUITimer :
	public Timer
{
public:
	juce_DeclareSingleton(ObjectUITimer, true);

	ObjectUITimer();
	~ObjectUITimer() {}

	Array<ObjectGridUI*, CriticalSection> uis;
	
	void registerUI(ObjectGridUI* uis);
	void unregisterUI(ObjectGridUI* uis);

	void timerCallback() override;
};