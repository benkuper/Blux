/*
  ==============================================================================

    StageLayoutManager.cpp
    Created: 6 Oct 2020 9:50:16pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(StageLayoutManager)

StageLayoutManager::StageLayoutManager() :
    Manager("Stage Layouts")
{
    iconSize = addFloatParameter("Icon Size", "Size of icons in view", 80,32,256);
    showFilters = addBoolParameter("Show Filters", "If checked, this will show filters in view", true);
    lockObjectUIs = addBoolParameter("Lock Objects", "If checked, this will lock objects in place", false);
    lockFilterUIs = addBoolParameter("Lock Filters", "If checked, this will lock filters in place", false);
    showSpat = addBoolParameter("Show Spat", "If checked, this will show spatializers in view", true);
}

StageLayoutManager::~StageLayoutManager()
{
}
