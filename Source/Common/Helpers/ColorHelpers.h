/*
  ==============================================================================

    ColorHelpers.h
    Created: 11 Aug 2023 5:07:09pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

//class RGBWAColour
//{
//public:
//    RGBWAColour(Colour c = Colours::black, float w = 0, float a = 0) : rgb(c), white(w), amber(a) {}
//
//    Colour rgb = Colours::black;
//    float white = 0;
//    float amber = 0;
//};


class ColorHelpers
{
public:
    static HashMap<int, Colour> temperatureColorMap;
    static void init();

    static var getRGBWFromRGB(Colour val, int temperature);
    static var getRGBWAFromRGB(Colour val);
};