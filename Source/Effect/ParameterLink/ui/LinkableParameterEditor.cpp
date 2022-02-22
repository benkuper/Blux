/*
  ==============================================================================

	LinkableParameterEditor.cpp
	Created: 21 Dec 2020 11:12:32pm
	Author:  bkupe

  ==============================================================================
*/

LinkableParameterEditor::LinkableParameterEditor(ParameterLink* pLink) :
	InspectableEditor(pLink->parameter.get(), false),
	link(pLink)

{
	link->addAsyncParameterLinkListener(this);

	linkBT.reset(AssetManager::getInstance()->getToggleBTImage(ImageCache::getFromMemory(BinaryData::link_png, BinaryData::link_pngSize)));
	linkBT->addListener(this);
	//linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);
	addAndMakeVisible(linkBT.get());

	paramEditor.reset((ParameterEditor*)pLink->parameter->getEditor(false));

	bool visible = link->linkType == link->NONE;
	if (visible)
	{
		addAndMakeVisible(paramEditor.get());
	}
	setSize(100, paramEditor->getHeight());
}

LinkableParameterEditor::~LinkableParameterEditor()
{
	if (!inspectable.wasObjectDeleted()) link->removeAsyncParameterLinkListener(this);
}

void LinkableParameterEditor::paint(Graphics& g)
{

	Colour c = NORMAL_COLOR;

	switch (link->linkType)
	{
	case ParameterLink::NONE:
		break;

	case ParameterLink::OBJECT_ID:
		c = BLUE_COLOR.withBrightness(.7f);
		break;

	case ParameterLink::CUSTOM_PARAM:
		c = GREEN_COLOR.withBrightness(.7f);
		break;
	}

	if (!paramEditor->isShowing())
	{
		Rectangle<int> r(paramEditor->getBounds());
		if (paramEditor->showLabel)
		{
			int controlSpace = jmax<int>(jmin<int>(getWidth() - 160, getWidth() * .75f), 100);
			g.setColour(c.brighter());
			g.drawFittedText(link->parameter->niceName, r.removeFromLeft(r.getWidth() - controlSpace), Justification::centredLeft, 1);
		}

		g.setColour(c.darker(.6f));
		g.fillRect(r);
		g.setColour(c.brighter(.5f));
		g.drawFittedText(getLinkLabel(), r.reduced(1), Justification::centred, 1);
	}


	g.setColour(c);
	g.fillEllipse(btRect.toFloat());
}

void LinkableParameterEditor::resized()
{
	Rectangle<int> r = getLocalBounds();
	int ts = jmin(r.getHeight(), 20);
	btRect = r.removeFromRight(ts).withHeight(ts).reduced(2);
	linkBT->setBounds(btRect);
	//linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);
	paramEditor->setBounds(r);
}

void LinkableParameterEditor::buttonClicked(Button* b)
{
	if (b == linkBT.get())
	{
		PopupMenu p;
		p.addItem(1, "Object ID", true, link->linkType == link->OBJECT_ID);
		p.addSeparator();

		p.addSectionHeader("Custom Parameters");
		int cpIndex = 100;
		for (auto& gci : ObjectManager::getInstance()->customParams.items)
		{
			p.addItem(cpIndex, gci->niceName, true, gci->controllable == link->linkedCustomParam);
			cpIndex++;
		}

		p.addSeparator();
		p.addItem(-1, "Unlink", link->linkType != link->NONE);

		p.showMenuAsync(PopupMenu::Options(), [this](int result)
			{
				if (result == -1) this->link->setLinkType(this->link->NONE);
				else if (result == 1) this->link->setLinkType(this->link->OBJECT_ID);
				else if (result >= 100) this->link->setLinkedCustomParam((Parameter*)ObjectManager::getInstance()->customParams.items[result - 100]->controllable);
			}
		);

	}
}

void LinkableParameterEditor::childBoundsChanged(Component* c)
{
	if (c == paramEditor.get())
	{
		setSize(getWidth(), paramEditor->getHeight());
		repaint();
	}
}

String LinkableParameterEditor::getLinkLabel() const
{
	String s = "";

	switch (link->linkType)
	{
	case ParameterLink::OBJECT_ID:
		s = "Object ID";
		break;
	case ParameterLink::CUSTOM_PARAM:
		s = "Custom : " + link->linkedCustomParam->niceName;
		break;

	}

	return s;

}

void LinkableParameterEditor::newMessage(const ParameterLink::ParameterLinkEvent& e)
{
	//linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);

	if (e.type == ParameterLink::ParameterLinkEvent::LINK_UPDATED || e.type == ParameterLink::ParameterLinkEvent::PREVIEW_UPDATED)
	{
		bool visible = link->linkType == link->NONE;
		if (visible) addAndMakeVisible(paramEditor.get());
		else removeChildComponent(paramEditor.get());
	}

	repaint();
}

ParamLinkContainerEditor::ParamLinkContainerEditor(ParamLinkContainer* container, bool isRoot, bool buildAtCreation) :
	GenericControllableContainerEditor(container, isRoot, false),
	paramLinkContainer(container),
	showLinkEditor(container->paramsCanBeLinked)
{
	if (buildAtCreation) resetAndBuild(); //force here to use the overriden getEditorUI function
}

ParamLinkContainerEditor::~ParamLinkContainerEditor()
{
}

InspectableEditor* ParamLinkContainerEditor::getEditorUIForControllable(Controllable* c)
{
	if (c->type == c->TRIGGER || !showLinkEditor) return  GenericControllableContainerEditor::getEditorUIForControllable(c);
	return new LinkableParameterEditor(paramLinkContainer->paramLinkMap[(Parameter*)c]);
}
