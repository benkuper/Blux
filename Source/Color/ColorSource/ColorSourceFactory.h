/*
  ==============================================================================

    ColorSourceFactory.h
    Created: 7 Nov 2020 5:26:47pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSourceFactory :
    public Factory<ColorSource>
{
public:
    juce_DeclareSingleton(ColorSourceFactory, true)

    ColorSourceFactory();
    ~ColorSourceFactory();

    void updateDefinitions();
};