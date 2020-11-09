/*
  ==============================================================================

    ColorSourceOverrideEffectEditor.cpp
    Created: 9 Nov 2020 4:23:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ColorSourceOverrideEffectEditor.h"

ColorSourceOverrideEffectEditor::ColorSourceOverrideEffectEditor(ColorSourceOverrideEffect* cef, bool isRoot) :
    EffectEditor(cef, isRoot),
    overrideEffect(cef)
{
    overrideEffect->addAsyncOverrideEffectListener(this);

    colorChooser.setButtonText(overrideEffect->colorSource != nullptr ? overrideEffect->colorSource->getSourceLabel() : "No Color Source");
    colorChooser.addChooserListener(this);
    addAndMakeVisible(&colorChooser);
}


ColorSourceOverrideEffectEditor::~ColorSourceOverrideEffectEditor()
{
    colorChooser.removeChooserListener(this);
    if(!inspectable.wasObjectDeleted()) overrideEffect->removeAsyncOverrideEffectListener(this);

}

void ColorSourceOverrideEffectEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    EffectEditor::resizedInternalHeaderItemInternal(r);
    colorChooser.setBounds(r.removeFromRight(100).reduced(2));
    r.removeFromRight(2);
}

void ColorSourceOverrideEffectEditor::sourceChosen(const String& type, ColorSource* templateRef)
{
    overrideEffect->setupSource(type, templateRef);
}

void ColorSourceOverrideEffectEditor::newMessage(const ColorSourceOverrideEffect::OverrideEffectEvent& e)
{
    if(e.type == e.SOURCE_CHANGED) colorChooser.setButtonText(overrideEffect->colorSource != nullptr ? overrideEffect->colorSource->getSourceLabel() : "No Color Source");
}
