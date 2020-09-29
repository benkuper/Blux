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

class ObjectManagerGridUI :
	public BaseManagerShapeShifterUI<ObjectManager, Object, ObjectGridUI>,
	public ContainerAsyncListener
{
public:
	ObjectManagerGridUI(const String& name);
	~ObjectManagerGridUI();


	std::unique_ptr<IntSliderUI> thumbSizeUI;

	void resizedInternalHeader(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int> &r) override;
	void newMessage(const ContainerAsyncEvent& e) override;

	static ObjectManagerGridUI* create(const String& name) { return new ObjectManagerGridUI(name); }
};