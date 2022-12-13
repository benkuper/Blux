/*
  ==============================================================================

    RawDataBlockManager.h
    Created: 13 Dec 2022 6:23:58pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class RawDataLayer;

class RawDataBlockManager :
    public LayerBlockManager
{
public:
    RawDataBlockManager(RawDataLayer * layer);
    ~RawDataBlockManager();

    LayerBlock* createItem() override;
};