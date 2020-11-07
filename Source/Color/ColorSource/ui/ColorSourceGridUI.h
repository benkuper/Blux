/*
  ==============================================================================

    ColorSourceGridUI.h
    Created: 3 Nov 2020 11:22:38am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ColorSource.h"

class ColorSourceGridUI :
    public BaseItemMinimalUI<ColorSource>
{
public:
    ColorSourceGridUI(ColorSource* item);
    ~ColorSourceGridUI();

    Image img;

    void paint(Graphics &g) override;
};