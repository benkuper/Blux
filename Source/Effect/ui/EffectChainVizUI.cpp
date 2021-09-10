/*
  ==============================================================================

    EffectChainVizUI.cpp
    Created: 30 Oct 2020 4:35:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectChainVizUI.h"
#include "Object/ObjectIncludes.h"

EffectChainVizUI::EffectChainVizUI(Effect* e, Object* o, ChainVizTarget::ChainVizType type) :
    BaseItemChainVizComponent(e, o, type),
    intensity("Out", "", 0, 0, 1),
    effect(e)
{
    intensity.setControllableFeedbackOnly(true);
    weightUI.reset(e->weight->createSlider());
    intensityUI.reset(intensity.createSlider());
    object->addInspectableListener(this);

    addAndMakeVisible(weightUI.get());
    addAndMakeVisible(intensityUI.get());
    startTimerHz(30);
    timerCallback(); //show directly
}

EffectChainVizUI::~EffectChainVizUI()
{
    if (object != nullptr) object->removeInspectableListener(this);
}

void EffectChainVizUI::resized()
{
    BaseItemChainVizComponent::resized();
    Rectangle<int> r = getLocalBounds().reduced(2);
    intensityUI->setBounds(r.removeFromBottom(16));

    r.removeFromLeft(30);
    weightUI->setBounds(r.removeFromTop(16));
}

void EffectChainVizUI::timerCallback()
{
    if (object == nullptr)
    {
        stopTimer();
        return;
    }

    if (object->effectIntensityOutMap.contains(effect))
    {
        intensity.setValue(object->effectIntensityOutMap[effect]);
    }
}