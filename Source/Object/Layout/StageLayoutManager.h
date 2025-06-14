/*
  ==============================================================================

    StageLayoutManager.h
    Created: 6 Oct 2020 9:50:16pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StageLayoutManager :
    public Manager<StageLayout>
{
public:
    juce_DeclareSingleton(StageLayoutManager, true);

    //ui
    FloatParameter* iconSize;
    BoolParameter * showFilters;
    BoolParameter* lockObjectUIs;
    BoolParameter* lockFilterUIs;
    BoolParameter* showSpat;

    StageLayoutManager();
    ~StageLayoutManager();
};