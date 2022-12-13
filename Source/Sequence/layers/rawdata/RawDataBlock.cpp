/*
  ==============================================================================

    RawDataBlock.cpp
    Created: 12 Dec 2022 5:38:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataBlock::RawDataBlock() :
    LayerBlock(getTypeString()),
    rawDataNotifier(5)
{
}

RawDataBlock::~RawDataBlock()
{
}
