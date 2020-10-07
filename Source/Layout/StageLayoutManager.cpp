/*
  ==============================================================================

    StageLayoutManager.cpp
    Created: 6 Oct 2020 9:50:16pm
    Author:  bkupe

  ==============================================================================
*/

#include "StageLayoutManager.h"

juce_ImplementSingleton(StageLayoutManager)

StageLayoutManager::StageLayoutManager() :
    BaseManager("Stage Layouts")
{
    iconSize = addFloatParameter("Icon Size", "Size of icons in view", 80,32,256);
    showFilters = addBoolParameter("Show Filters", "If checked, this will show filters in view", true);
}

StageLayoutManager::~StageLayoutManager()
{
}
