/*
  ==============================================================================

    ColorSourceGridUI.cpp
    Created: 3 Nov 2020 11:22:38am
    Author:  bkupe

  ==============================================================================
*/

#include "ColorSourceGridUI.h"
#include "Object//ObjectManager.h"
#include "Object/Component/components/color/ColorComponent.h"

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

void ColorSourceGridUI::mouseDown(const MouseEvent& e)
{
    BaseItemMinimalUI::mouseDown(e);

    if (e.mods.isRightButtonDown())
    {
        PopupMenu m;
        m.addItem(1, "Assign to all");
        m.addItem(2, "Assign to selection");

        if (int result = m.show())
        {
            if (result == 1 || result == 2)
            {
                Array<Object*> objects;
                if (result == 1) objects.addArray(ObjectManager::getInstance()->items.getRawDataPointer(), ObjectManager::getInstance()->items.size());
                else objects = InspectableSelectionManager::mainSelectionManager->getInspectablesAs<Object>();

                for (auto& o : objects)
                {
                    if (ColorComponent* c = o->getComponent<ColorComponent>())
                    {
                        c->setupSource(item->getTypeString(), item);
                    } 
                }
            }
        }
    }
}
