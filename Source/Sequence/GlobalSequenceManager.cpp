/*
  ==============================================================================

    GlobalSequenceManager.cpp
    Created: 5 Oct 2020 2:42:12pm
    Author:  bkupe

  ==============================================================================
*/

#include "GlobalSequenceManager.h"

juce_ImplementSingleton(GlobalSequenceManager);

GlobalSequenceManager::GlobalSequenceManager()
{
    setNiceName("Global Sequences");
}

GlobalSequenceManager::~GlobalSequenceManager()
{
}
