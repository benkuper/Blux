/*
  ==============================================================================

    GlobalSequenceManager.h
    Created: 5 Oct 2020 2:42:12pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "BluxSequenceManager.h"

class GlobalSequenceManager :
    public BluxSequenceManager
{
public:
    juce_DeclareSingleton(GlobalSequenceManager, true);

    GlobalSequenceManager();
    ~GlobalSequenceManager();
};