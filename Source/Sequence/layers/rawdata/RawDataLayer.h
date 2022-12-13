/*
  ==============================================================================

    RawDataLayer.h
    Created: 12 Dec 2022 5:37:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class RawDataLayer :
    public SequenceLayer
{
public:
    RawDataLayer(Sequence* s, var params = var());
    ~RawDataLayer();

    RawDataBlockManager blockManager;
    Array<RawDataBlock*, CriticalSection> activeBlocks;

    SequenceLayerTimeline* getTimelineUI() override;

    DECLARE_TYPE("Raw Data");
    static RawDataLayer* create(Sequence* s, var params) { return new RawDataLayer(s, params); }

};