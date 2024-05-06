/*
  ==============================================================================

	EffectManagerEditor.h
	Created: 30 Sep 2020 12:39:05pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class EffectManagerEditor : public GenericManagerEditor<Effect>
{
public:
	EffectManagerEditor(EffectManager* manager, bool isRoot = false);
	~EffectManagerEditor() {}

	std::unique_ptr<FloatSliderUI> weightUI;

	void resizedInternalHeader(Rectangle<int>& r) override;
};