/*
  ==============================================================================

    BluxSequenceManager.cpp
    Created: 5 Oct 2020 2:42:03pm
    Author:  bkupe

  ==============================================================================
*/

#include "BluxSequenceManager.h"
#include "Object/Object.h"

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
    for (auto& i : items) result.addArray(((BluxSequence*)i)->getChainVizTargetsForObject(o));
    return result;
}

void BluxSequenceManager::processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier)
{
    for (auto& i : items) ((BluxSequence *)i)->processComponentValues(o, c, values, weightMultiplier);
}

Sequence* BluxSequenceManager::createItem()
{
    return new BluxSequence();
}
