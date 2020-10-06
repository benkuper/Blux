/*
  ==============================================================================

    StageLayout.h
    Created: 6 Oct 2020 9:37:41pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class StageLayout :
    public BaseItem
{
public:
    StageLayout();
    ~StageLayout();

    var layoutData;

    void saveLayout();

    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;
};