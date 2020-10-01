/*
  ==============================================================================

    EffectEditor.cpp
    Created: 30 Sep 2020 12:38:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectEditor.h"

EffectEditor::EffectEditor(Effect* effect, bool isRoot) :
    BaseItemEditor(effect, isRoot),
    effect(effect)
{
    weightUI.reset(effect->weight->createSlider());
    addAndMakeVisible(weightUI.get());
}

EffectEditor::~EffectEditor()
{
}

void EffectEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    weightUI->setBounds(r.removeFromRight(100).reduced(2));
}
