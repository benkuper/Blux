/*
  ==============================================================================

    Main.h
    Created: 26 Sep 2020 10:00:55am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
class BluxApplication : public OrganicApplication
{
public:
    //==============================================================================
    BluxApplication();

    void initialiseInternal(const String &commandLine) override;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(BluxApplication)