/*
  ==============================================================================

    StageLayoutManager.h
    Created: 6 Oct 2020 9:50:16pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "StageLayout.h"

class StageLayoutManager :
    public BaseManager<StageLayout>
{
public:
    juce_DeclareSingleton(StageLayoutManager, true);

    //ui
    FloatParameter* iconSize;
    BoolParameter * showFilters;

    StageLayoutManager();
    ~StageLayoutManager();
};