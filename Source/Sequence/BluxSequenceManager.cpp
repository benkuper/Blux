/*
  ==============================================================================

    BluxSequenceManager.cpp
    Created: 5 Oct 2020 2:42:03pm
    Author:  bkupe

  ==============================================================================
*/

#include "BluxSequenceManager.h"

BluxSequenceManager::BluxSequenceManager()
{
    itemDataType = "Sequence";
    helpID = "Sequence";
}

BluxSequenceManager::~BluxSequenceManager()
{
}

Sequence* BluxSequenceManager::createItem()
{
    return new BluxSequence();
}
