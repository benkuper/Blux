/*
  ==============================================================================

    BluxSequence.h
    Created: 5 Oct 2020 2:42:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class BluxSequence :
    public Sequence
{
public:
    BluxSequence();
    ~BluxSequence();

    String getTypeString() const override { return "Blux Sequence"; }
};