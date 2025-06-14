/*
  ==============================================================================

	GlobalEffectManagerUI.h
	Created: 30 Sep 2020 1:37:01pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class EffectGroupUI : public ItemUI<EffectGroup>
{
public:
	EffectGroupUI(EffectGroup* group);
	~EffectGroupUI() {}

	std::unique_ptr<FloatSliderUI> weightUI;

	void resizedInternalHeader(Rectangle<int>& r) override;
};

class GlobalEffectManagerUI :
	public ManagerShapeShifterUI<GlobalEffectManager, EffectGroup, EffectGroupUI>
{
public:
	GlobalEffectManagerUI(const String& name);
	~GlobalEffectManagerUI();

	static GlobalEffectManagerUI* create(const String& name) { return new GlobalEffectManagerUI(name); }
};