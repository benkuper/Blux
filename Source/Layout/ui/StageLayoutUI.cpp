/*
  ==============================================================================

    StageLayoutUI.cpp
    Created: 9 Oct 2020 8:53:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "StageLayoutUI.h"

StageLayoutUI::StageLayoutUI(StageLayout* item) :
    BaseItemUI(item)
{
}

StageLayoutUI::~StageLayoutUI()
{
}

void StageLayoutUI::mouseDown(const MouseEvent& e)
{
    BaseItemUI::mouseDown(e);

    if (e.mods.isRightButtonDown())
    {
        PopupMenu m;
        m.addItem(1, "Save");
        m.addItem(2, "Load");

        int result = m.show();

        if (result == 1) item->saveLayout();
        else if (result == 2) item->loadLayout();
    }
}