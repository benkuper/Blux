/*
  ==============================================================================

    GlobalSequenceManager.cpp
    Created: 5 Oct 2020 2:42:12pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(GlobalSequenceManager);

GlobalSequenceManager::GlobalSequenceManager()
{
    setNiceName("Global Sequences");
}

GlobalSequenceManager::~GlobalSequenceManager()
{
}
