/*
  ==============================================================================

    ObjectManagerGridUI.h
    Created: 26 Sep 2020 7:51:37pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "ObjectGridUI.h"
#include "../ObjectManager.h"
#include "Scene/SceneManager.h"

class ObjectManagerGridUI :
	public BaseManagerShapeShifterUI<ObjectManager, Object, ObjectGridUI>,
	public ContainerAsyncListener,
	public SceneManager::AsyncSceneListener
{
public:
	ObjectManagerGridUI(const String& name);
	~ObjectManagerGridUI();


	std::unique_ptr<IntSliderUI> thumbSizeUI;
	std::unique_ptr<FloatSliderUI> flashValueUI;
	std::unique_ptr<BoolButtonToggleUI> blackOutUI;
	std::unique_ptr<BoolButtonToggleUI> activeInSceneUI;

	void resizedInternalHeader(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void setPreviewData(var data = var());

	bool hasFiltering() override;
	bool checkFilterForItem(ObjectGridUI* ui) override;

	void newMessage(const ContainerAsyncEvent& e) override;
	void newMessage(const SceneManager::SceneManagerEvent& e) override;

	static ObjectManagerGridUI* create(const String& name) { return new ObjectManagerGridUI(name); }
};