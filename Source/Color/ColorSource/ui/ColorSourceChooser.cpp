/*
  ==============================================================================

	ColorSourceChooser.cpp
	Created: 8 Nov 2020 10:04:28am
	Author:  bkupe

  ==============================================================================
*/

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

		ColorSource* refColorSource = result < -1 ? ColorSourceLibrary::getInstance()->items[result + 10000] : nullptr;
		String type = refColorSource != nullptr ? refColorSource->getTypeString() : (result > 0 ? ColorSourceFactory::getInstance()->defs[result - 1]->type : "");

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
	addSeparator();
	addItem(-1,"Remove");
}
