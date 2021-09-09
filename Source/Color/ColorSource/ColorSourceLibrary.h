/*
  ==============================================================================

    ColorSourceLibrary.h
    Created: 2 Nov 2020 7:13:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSourceLibrary :
    public BaseManager<ColorSource>
{
public:
    juce_DeclareSingleton(ColorSourceLibrary, true);

    ColorSourceLibrary();
    ~ColorSourceLibrary();

    //ui
    IntParameter* gridThumbSize;
};