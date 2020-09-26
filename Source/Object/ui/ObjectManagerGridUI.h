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
	public BaseManagerShapeShifterUI<ObjectManager, Object, ObjectGridUI>
{
public:
	ObjectManagerGridUI(const String& name);
	~ObjectManagerGridUI();

	int thumbSize = 128;
	const int gap = 2;
	const int headerHeight = 16;
	const int headerGap = 4;

	void resizedInternalContent(Rectangle<int> &r) override;

	void setThumbSize(int value);

	static ObjectManagerGridUI* create(const String& name) { return new ObjectManagerGridUI(name); }
};