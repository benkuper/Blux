/*
  ==============================================================================

    ObjectManagerGridUI.h
    Created: 26 Sep 2020 7:51:37pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ObjectManagerGridUI :
	public ManagerShapeShifterUI<ObjectManager, Object, ObjectGridUI>,
	public ContainerAsyncListener,
	public AsyncSceneListener
{
public:
	ObjectManagerGridUI(const String& name);
	~ObjectManagerGridUI();


	std::unique_ptr<IntSliderUI> thumbSizeUI;
	std::unique_ptr<FloatSliderUI> flashValueUI;
	std::unique_ptr<BoolButtonToggleUI> blackOutUI;
	std::unique_ptr<BoolButtonToggleUI> activeInSceneUI;

	void paint(Graphics& g) override;

	void resizedInternalHeader(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void setPreviewData(var data = var());

	bool hasFiltering() override;
	bool checkFilterForItem(ObjectGridUI* ui) override;

	void newMessage(const ContainerAsyncEvent& e) override;
	void newMessage(const SceneManagerEvent& e) override;

	int getDropIndexForPosition(Point<int> localPosition) override;

	static ObjectManagerGridUI* create(const String& name) { return new ObjectManagerGridUI(name); }
};