/*
  ==============================================================================

	InterfaceUI.cpp
	Created: 26 Sep 2020 7:47:24pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface/InterfaceIncludes.h"
#include "UI/AssetManager.h"

InterfaceUI::InterfaceUI(Interface* item) :
	ItemUI(item)
{
	inActivityUI.reset(item->inActivityTrigger->createImageUI(AssetManager::getInstance()->inImage));
	inActivityUI->showLabel = false;
	addAndMakeVisible(inActivityUI.get());

	outActivityUI.reset(item->outActivityTrigger->createImageUI(AssetManager::getInstance()->outImage));
	outActivityUI->showLabel = false;
	addAndMakeVisible(outActivityUI.get());

	updateConnectedUI();

	item->addAsyncCoalescedInterfaceListener(this);
}

InterfaceUI::~InterfaceUI()
{
	item->removeAsyncInterfaceListener(this);
}

void InterfaceUI::paintOverChildren(Graphics& g)
{
	ItemUI::paintOverChildren(g);

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
	ItemUI::resizedHeader(r);

	outActivityUI->setBounds(r.removeFromRight(r.getHeight()));
	inActivityUI->setBounds(r.removeFromRight(r.getHeight()));
	if (connectedUI != nullptr)
	{
		connectedUI->setBounds(r.removeFromRight(r.getHeight()));
	}
}

void InterfaceUI::mouseDown(const MouseEvent& e)
{
	ItemUI::mouseDown(e);
	if (e.eventComponent == inActivityUI.get()) item->logIncomingData->setValue(!item->logIncomingData->boolValue());
	else if (e.eventComponent == outActivityUI.get()) item->logOutgoingData->setValue(!item->logOutgoingData->boolValue());
}

void InterfaceUI::updateConnectedUI()
{
	if (BoolParameter* p = item->getConnectedParam())
	{
		if (connectedUI != nullptr && connectedUI->parameter == p) return;

		if (p == nullptr)
		{
			removeChildComponent(connectedUI.get());
			connectedUI.reset();
		}
		else
		{
			connectedUI.reset(p->createToggle(BluxAssetManager::getImage("connected"), BluxAssetManager::getImage("disconnected")));
			addAndMakeVisible(connectedUI.get());
		}
	}

	resized();
}

void InterfaceUI::newMessage(const Interface::InterfaceEvent& e)
{
	switch (e.type)
	{
	case Interface::InterfaceEvent::DEVICE_CHANGED:
		updateConnectedUI();
		break;
	}
}


