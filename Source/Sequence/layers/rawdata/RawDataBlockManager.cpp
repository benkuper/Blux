/*
  ==============================================================================

	RawDataBlockManager.cpp
	Created: 13 Dec 2022 6:23:58pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataBlockManager::RawDataBlockManager(RawDataLayer* layer) :
	LayerBlockManager(layer, "Blocks")
{
}

RawDataBlockManager::~RawDataBlockManager()
{
}

LayerBlock* RawDataBlockManager::createItem()
{
	return new RawDataBlock();
}