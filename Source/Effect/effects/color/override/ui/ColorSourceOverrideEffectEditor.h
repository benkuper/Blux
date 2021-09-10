/*
  ==============================================================================

    ColorSourceOverrideEffectEditor.h
    Created: 9 Nov 2020 4:23:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSourceOverrideEffectEditor :
    public EffectEditor,
    public ColorSourceChooser::ChooserListener,
    public ColorSourceOverrideEffect::AsyncOverrideEffectListener
{
public:
    ColorSourceOverrideEffectEditor(ColorSourceOverrideEffect* cef, bool isRoot);
    ~ColorSourceOverrideEffectEditor();

    ColorSourceOverrideEffect* overrideEffect;

    ColorSourceChooser colorChooser;
    void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

    void sourceChosen(const String& type, ColorSource* templateRef) override;
    void newMessage(const ColorSourceOverrideEffect::OverrideEffectEvent & e) override;
};