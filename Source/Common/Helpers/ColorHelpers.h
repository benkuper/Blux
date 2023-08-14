/*
  ==============================================================================

    ColorHelpers.h
    Created: 11 Aug 2023 5:07:09pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class ColorHelpers
{
public:
    static HashMap<int, Colour> temperatureColorMap;
    static void init();
    static Colour getColorForTemperature(float temperature);

    static var getRGBWFromRGB(Colour val, float temperature);
    static var getRGBWAFromRGB(Colour val, float temperature);
};