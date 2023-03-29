/*
  ==============================================================================

	InterfaceUI.cpp
	Created: 26 Sep 2020 7:47:24pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface/InterfaceIncludes.h"

InterfaceUI::InterfaceUI(Interface* item) :
	BaseItemUI(item)
{
	inActivityUI.reset(item->inActivityTrigger->createImageUI(AssetManager::getInstance()->inImage));
	inActivityUI->showLabel = false;
	addAndMakeVisible(inActivityUI.get());

	outActivityUI.reset(item->outActivityTrigger->createImageUI(AssetManager::getInstance()->outImage));
	outActivityUI->showLabel = false;
	addAndMakeVisible(outActivityUI.get());
}

InterfaceUI::~InterfaceUI()
{
}

void InterfaceUI::paintOverChildren(Graphics& g)
{
	BaseItemUI::paintOverChildren(g);

	if (item->logIncomingData != nullptr && item->logIncomingData->boolValue())
	{
		g.setColour(BLUE_COLOR.brighter(.3f));
		g.drawEllipse(inActivityUI->getBounds().toFloat().reduced(1), 1.5f);
	}

	if (item->logOutgoingData != nullptr && item->logOutgoingData->boolValue())
	{
		g.setColour(GREEN_COLOR);
		g.drawEllipse(outActivityUI->getBounds().toFloat().reduced(1), 1);
	}
}

void InterfaceUI::resizedHeader(Rectangle<int>& r)
{
	BaseItemUI::resizedHeader(r);

	outActivityUI->setBounds(r.removeFromRight(r.getHeight()));
	inActivityUI->setBounds(r.removeFromRight(r.getHeight()));
}

void InterfaceUI::mouseDown(const MouseEvent& e)
{
	BaseItemUI::mouseDown(e);
	if (e.eventComponent == inActivityUI.get()) item->logIncomingData->setValue(!item->logIncomingData->boolValue());
	else if (e.eventComponent == outActivityUI.get()) item->logOutgoingData->setValue(!item->logOutgoingData->boolValue());
}


