/*
  ==============================================================================

	ColorSourceChooser.cpp
	Created: 8 Nov 2020 10:04:28am
	Author:  bkupe

  ==============================================================================
*/

#include "ColorSourceChooser.h"
#include "Color/ColorSource/ColorSourceFactory.h"
#include "Color/ColorSource/ColorSourceLibrary.h"

ColorSourceChooser::ColorSourceChooser() :
	TextButton("No Color Source")
{
}

ColorSourceChooser::~ColorSourceChooser()
{
}

void ColorSourceChooser::clicked()
{
	ColorSourceMenu m;
	if (int result = m.show())
	{

		ColorSource* refColorSource = result < 0 ? ColorSourceLibrary::getInstance()->items[result + 10000] : nullptr;
		String type = refColorSource != nullptr ? refColorSource->getTypeString() : ColorSourceFactory::getInstance()->defs[result - 1]->type;

		chooserListeners.call(&ChooserListener::sourceChosen, type, refColorSource);
	}
}

ColorSourceMenu::ColorSourceMenu() :
	PopupMenu(ColorSourceFactory::getInstance()->getMenu())
{
	
	PopupMenu tm;
	int index = -10000;
	for (auto& i : ColorSourceLibrary::getInstance()->items)
	{
		tm.addItem(index++, i->niceName);
	}

	addSeparator();
	addSubMenu("Templates", tm);
}
