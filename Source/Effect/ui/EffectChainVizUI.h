/*
  ==============================================================================

    EffectChainVizUI.h
    Created: 30 Oct 2020 4:35:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Effect;

class EffectChainVizUI :
    public BaseItemChainVizComponent,
    public Timer
{
public:
    EffectChainVizUI(Effect * e, Object* o, ComponentType ct, ChainVizTarget::ChainVizType type);
    ~EffectChainVizUI();

    Effect* effect;
    WeakReference<Inspectable> effectRef;
    
    std::unique_ptr<FloatSliderUI> weightUI;

    std::unique_ptr<Parameter> param;
    std::unique_ptr<ParameterUI> paramUI;

    virtual bool isReallyAffecting();
    virtual String getVizLabel() const override;


    void resized() override;
    void timerCallback() override;
};