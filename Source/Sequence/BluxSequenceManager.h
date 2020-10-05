/*
  ==============================================================================

    BluxSequenceManager.h
    Created: 5 Oct 2020 2:42:03pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "BluxSequence.h"

class BluxSequenceManager :
    public SequenceManager
{
public:
    BluxSequenceManager();
    ~BluxSequenceManager();

    Sequence* createItem();
};