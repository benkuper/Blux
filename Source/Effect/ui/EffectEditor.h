/*
  ==============================================================================

    EffectEditor.h
    Created: 30 Sep 2020 12:38:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class EffectEditor :
    public BaseItemEditor
{
public:
    EffectEditor(Effect* effect, bool isRoot);
    virtual ~EffectEditor();

    Effect* effect;
    std::unique_ptr<FloatSliderUI> weightUI;

    void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
};