/*
  ==============================================================================

    BluxSequenceManager.h
    Created: 5 Oct 2020 2:42:03pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "BluxSequence.h"
class ChainVizTarget;

class BluxSequenceManager :
    public SequenceManager
{
public:
    BluxSequenceManager();
    ~BluxSequenceManager();

    Array<ChainVizTarget *> getChainVizTargetsForObject(Object* o);
    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f);

    Sequence* createItem();
};