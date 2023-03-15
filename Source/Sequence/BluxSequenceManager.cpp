/*
  ==============================================================================

    BluxSequenceManager.cpp
    Created: 5 Oct 2020 2:42:03pm
    Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

BluxSequenceManager::BluxSequenceManager()
{
    itemDataType = "Sequence";
    helpID = "Sequence";
}

BluxSequenceManager::~BluxSequenceManager()
{
}

Array<ChainVizTarget *> BluxSequenceManager::getChainVizTargetsForObject(Object* o)
{
    Array<ChainVizTarget *> result;
    for (auto& i : items)
    {
        BluxSequence* bs = (BluxSequence*)i;
        if (bs->isAffectingObject(o)) result.add(bs);
    }
    return result;
}

void BluxSequenceManager::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier)
{
    for (auto& i : items)
    {
        if (!i->enabled->boolValue()) return;
        ((BluxSequence*)i)->processComponent(o, c, values, weightMultiplier);
    }
}

Sequence* BluxSequenceManager::createItem()
{
    return new BluxSequence();
}
