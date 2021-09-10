/*
  ==============================================================================

    EffectLayerTimeline.h
    Created: 10 Oct 2020 10:48:01am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class EffectLayerTimeline :
    public SequenceLayerTimeline
{
public:
    EffectLayerTimeline(EffectLayer* l);
    ~EffectLayerTimeline();

    EffectLayer* effectLayer;
    EffectBlockManagerUI blockManagerUI;

	void resized() override;
	void updateContent() override;
	virtual void updateMiniModeUI() override;

	virtual void addSelectableComponentsAndInspectables(Array<Component*>& selectables, Array<Inspectable*>& inspectables) override;
};
