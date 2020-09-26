/*
  ==============================================================================

    BluxEngine.h
    Created: 26 Sep 2020 10:01:11am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class BluxEngine : public Engine
{
public:
    BluxEngine();
    ~BluxEngine();

    void clearInternal() override;

    var getJSONData() override;
    void loadJSONDataInternalEngine(var data, ProgressTask * task) override;
};