/*
  ==============================================================================

	GlobalEffectManagerUI.cpp
	Created: 30 Sep 2020 1:37:01pm
	Author:  bkupe

  ==============================================================================
*/

GlobalEffectManagerUI::GlobalEffectManagerUI(const String& name) :
	BaseManagerShapeShifterUI(name, GlobalEffectManager::getInstance())
{
	setShowSearchBar(true);
	addExistingItems();
}

GlobalEffectManagerUI::~GlobalEffectManagerUI()
{
}

EffectGroupUI::EffectGroupUI(EffectGroup* item) :
	BaseItemUI(item)
{
	weightUI.reset(item->effectManager.globalWeight->createSlider());
	addAndMakeVisible(weightUI.get());
}

void EffectGroupUI::resizedInternalHeader(Rectangle<int>& r)
{
	BaseItemUI::resizedInternalHeader(r);
	r.removeFromRight(2);
	r.removeFromLeft(2);
	weightUI->setBounds(r.removeFromRight(100));
}
