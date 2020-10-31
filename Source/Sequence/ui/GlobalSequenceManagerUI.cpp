/*
  ==============================================================================

    GlobalSequenceManagerUI.cpp
    Created: 5 Oct 2020 2:42:26pm
    Author:  bkupe

  ==============================================================================
*/

#include "GlobalSequenceManagerUI.h"

GlobalSequenceManagerUI::GlobalSequenceManagerUI(const String& name) :
    SequenceManagerUI(name, GlobalSequenceManager::getInstance())
{
   // addExistingItems();
}

GlobalSequenceManagerUI::~GlobalSequenceManagerUI()
{
}
