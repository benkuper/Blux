/*
  ==============================================================================

    EffectChainVizUI.h
    Created: 30 Oct 2020 4:35:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ChainViz/ChainViz.h"
class Effect;

class EffectChainVizUI :
    public BaseItemChainVizComponent,
    public Timer
{
public:
    EffectChainVizUI(Effect * e, Object* o, ChainVizTarget::ChainVizType type);
    ~EffectChainVizUI();

    Effect* effect;
    
    std::unique_ptr<FloatSliderUI> weightUI;

    FloatParameter intensity;
    std::unique_ptr<FloatSliderUI> intensityUI;

    void resized() override;
    void timerCallback() override;
};