/*
  ==============================================================================

    RawDataLayerTimeline.h
    Created: 13 Dec 2022 6:23:39pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class RawDataLayerTimeline :
    public SequenceLayerTimeline
{
public:
    RawDataLayerTimeline(RawDataLayer* l);
    ~RawDataLayerTimeline();

    RawDataLayer* rawDataLayer;
    RawDataBlockManagerUI blockManagerUI;

    void resized() override;
    void updateContent() override;
    virtual void updateMiniModeUI() override;

    virtual void addSelectableComponentsAndInspectables(Array<Component*>& selectables, Array<Inspectable*>& inspectables) override;
};