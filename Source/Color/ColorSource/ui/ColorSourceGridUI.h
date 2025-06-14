/*
  ==============================================================================

    ColorSourceGridUI.h
    Created: 3 Nov 2020 11:22:38am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSourceGridUI :
    public ItemMinimalUI<ColorSource>
{
public:
    ColorSourceGridUI(ColorSource* item);
    ~ColorSourceGridUI();

    Image img;

    void paint(Graphics &g) override;

    void mouseDown(const MouseEvent& e) override;
};