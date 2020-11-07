/*
  ==============================================================================

    ColorSourceGridUI.cpp
    Created: 3 Nov 2020 11:22:38am
    Author:  bkupe

  ==============================================================================
*/

#include "ColorSourceGridUI.h"

ColorSourceGridUI::ColorSourceGridUI(ColorSource* item) :
    BaseItemMinimalUI(item)
{
    if (item->imgPath.existsAsFile())
    {
        img = ImageCache::getFromFile(item->imgPath);
    }
    else
    {
        int imgSize = 0;
        String iconName = item->getTypeString().replace(" ", "").replace("/", "").toLowerCase() + "_png";
        const char* imgData = BinaryData::getNamedResource(iconName.getCharPointer(), imgSize);
        if (imgData != nullptr) img = ImageCache::getFromMemory(imgData, imgSize);
        else img = ImageCache::getFromMemory(BinaryData::icon2_png, BinaryData::icon2_pngSize);
    }
}
   

ColorSourceGridUI::~ColorSourceGridUI()
{

}

void ColorSourceGridUI::paint(Graphics& g)
{
    g.setColour(bgColor);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);

    g.setColour(Colours::white.withAlpha(isMouseOver() ? .5f : 1.f));
    g.drawImage(img, getLocalBounds().reduced(8).toFloat());

    if (!img.isValid() || isMouseOver())
    {
        g.setColour(Colours::white);
        g.drawFittedText(item->niceName, getLocalBounds().reduced(4), Justification::centred, 3);
    }
}
