/*
  ==============================================================================

    ColorSourceLibrary.cpp
    Created: 2 Nov 2020 7:13:43pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(ColorSourceLibrary)


ColorSourceLibrary::ColorSourceLibrary() :
    Manager("Color Templates")
{
    gridThumbSize = addIntParameter("Thumb Size", "Size of thumbnails in grid view", 64, 32, 256);
    managerFactory = ColorSourceFactory::getInstance();
}

ColorSourceLibrary::~ColorSourceLibrary()
{
}
