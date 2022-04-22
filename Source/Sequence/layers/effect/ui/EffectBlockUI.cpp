/*
  ==============================================================================

	EffectBlockUI.cpp
	Created: 10 Oct 2020 10:47:39am
	Author:  bkupe

  ==============================================================================
*/

EffectBlockUI::EffectBlockUI(EffectBlock* b) :
	LayerBlockUI(b),
	effectBlock(b)
{

	addChildComponent(&fadeInHandle, 0);
	addChildComponent(&fadeOutHandle, 0);

	fadeInHandle.setVisible(effectBlock->fadeIn->enabled);
	fadeOutHandle.setVisible(effectBlock->fadeOut->enabled);

	fadeInHandle.addMouseListener(this, false);
	fadeOutHandle.addMouseListener(this, false);

	bgColor = BG_COLOR.darker(.4f);

	effectBlock->addAsyncEffectBlockListener(this);
}

EffectBlockUI::~EffectBlockUI()
{
	if (!inspectable.wasObjectDeleted()) effectBlock->removeAsyncEffectBlockListener(this);
}

void EffectBlockUI::setTargetAutomation(ParameterAutomation* a)
{
	if (automationUI != nullptr)
	{
		removeChildComponent(automationUI.get());
		automationUI = nullptr;
	}

	canBeGrabbed = true;

	if (a == nullptr) return;


	if (dynamic_cast<ParameterNumberAutomation*>(a) != nullptr)
	{
		AutomationUI* aui = new AutomationUI((Automation*)a->automationContainer);
		//aui->updateROI();
		aui->showMenuOnRightClick = false;
		automationUI.reset(aui);
	}
	else if (dynamic_cast<ParameterColorAutomation*>(a) != nullptr)
	{
		GradientColorManagerUI* gui = new GradientColorManagerUI((GradientColorManager*)a->automationContainer);
		gui->autoResetViewRangeOnLengthUpdate = true;
		automationUI.reset(gui);
	}

	if (automationUI != nullptr)
	{
		canBeGrabbed = false;
		coreGrabber.setVisible(false);
		grabber.setVisible(false);
		loopGrabber.setVisible(false);
		automationUI->addMouseListener(this, true);
		addAndMakeVisible(automationUI.get());
		resized();
	}
}

void EffectBlockUI::paint(Graphics& g)
{
	LayerBlockUI::paint(g);
	g.setColour(bgColor.brighter());
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 1);

	g.setColour(TEXT_COLOR);
	g.setFont(g.getCurrentFont().withHeight(jlimit<float>(10, 20, getHeight() - 20)).boldened());
	g.drawText(effectBlock->niceName, getCoreBounds().toFloat(), Justification::centred);
}

void EffectBlockUI::paintOverChildren(Graphics& g)
{
	LayerBlockUI::paintOverChildren(g);

	Colour fInColor = (effectBlock->fadeIn->enabled ? NORMAL_COLOR : BLUE_COLOR).withAlpha(.5f);
	Colour fOutColor = (effectBlock->fadeOut->enabled ? NORMAL_COLOR : BLUE_COLOR).withAlpha(.5f);

	if (effectBlock->fadeIn->floatValue() > 0)
	{
		g.setColour(fInColor);
		g.drawLine(0, getHeight(), getWidth() * (effectBlock->fadeIn->floatValue() / effectBlock->getTotalLength()), fadeInHandle.getY() + fadeInHandle.getHeight() / 2);
	}

	if (effectBlock->fadeOut->floatValue() > 0)
	{
		g.setColour(fOutColor);
		g.drawLine(getWidth() * (1 - (effectBlock->fadeOut->floatValue() / effectBlock->getTotalLength())), fadeOutHandle.getY() + fadeOutHandle.getHeight() / 2, getWidth(), getHeight());
	}

}

void EffectBlockUI::resizedBlockInternal()
{

	if (automationUI != nullptr)
	{
		Rectangle<int> r = getCoreBounds();
		if (automationUI != nullptr)
		{
			if (dynamic_cast<GradientColorManagerUI*>(automationUI.get()) != nullptr) automationUI->setBounds(r.removeFromBottom(20));
			else automationUI->setBounds(r);
		}
	}


	fadeInHandle.setCentrePosition((effectBlock->fadeIn->floatValue() / effectBlock->getTotalLength()) * getWidth(), fadeInHandle.getHeight() / 2);
	fadeOutHandle.setCentrePosition((1 - effectBlock->fadeOut->floatValue() / effectBlock->getTotalLength()) * getWidth(), fadeOutHandle.getHeight() / 2);
}


void EffectBlockUI::mouseDown(const MouseEvent& e)
{
	LayerBlockUI::mouseDown(e);

	if (e.eventComponent == &fadeInHandle) fadeValueAtMouseDown = effectBlock->fadeIn->floatValue();
	else if (e.eventComponent == &fadeOutHandle) fadeValueAtMouseDown = effectBlock->fadeOut->floatValue();

	if (e.mods.isRightButtonDown() && (e.eventComponent == this || e.eventComponent == automationUI.get()))
	{
		PopupMenu p;
		p.addItem(1, "Clear automation editor");

		PopupMenu ap;

		Array<WeakReference<Parameter>> params = effectBlock->effect->effectParams.getAllParameters(true);

		int index = 2;
		for (auto& pa : params)
		{
			if (pa->canBeAutomated) ap.addItem(index, pa->niceName, true, pa->controlMode == Parameter::ControlMode::AUTOMATION);
			index++;
		}

		p.addSubMenu("Edit...", ap);

		p.showMenuAsync(PopupMenu::Options(), [this, params](int result)
			{
				if (result > 0)
				{
					if (result == 1) setTargetAutomation(nullptr);
					else
					{
						WeakReference<Parameter> pa = params[result - 2];
						if (pa->controlMode != Parameter::ControlMode::AUTOMATION)
						{
							pa->setControlMode(Parameter::ControlMode::AUTOMATION);
							pa->automation->setManualMode(true);
							Automation* a = dynamic_cast<Automation*>(pa->automation->automationContainer);
							if (a != nullptr)
							{
								a->clear();
								AutomationKey* k = a->addItem(0, 0);
								k->setEasing(Easing::BEZIER);
								a->addKey(a->length->floatValue(), 1);
							}
						}

						if (!pa.wasObjectDeleted()) setTargetAutomation(pa->automation.get());
					}
				}
			}
		);
	}
}

void EffectBlockUI::mouseDrag(const MouseEvent& e)
{
	LayerBlockUI::mouseDrag(e);

	if (e.eventComponent == &fadeInHandle)
	{
		effectBlock->fadeIn->setValue((getMouseXYRelative().x * 1.0f / getWidth()) * effectBlock->getTotalLength());
		resized();
	}
	else if (e.eventComponent == &fadeOutHandle)
	{
		effectBlock->fadeOut->setValue((1 - (getMouseXYRelative().x * 1.0f / getWidth())) * effectBlock->getTotalLength());
		resized();
	}

	if (e.eventComponent == automationUI.get() && e.mods.isLeftButtonDown()) //because recursive mouseListener is removed to have special handling of automation
	{
		item->selectThis();
	}
}

void EffectBlockUI::mouseUp(const MouseEvent& e)
{
	LayerBlockUI::mouseUp(e);

	if (e.eventComponent == &fadeInHandle)
	{
		effectBlock->fadeIn->setUndoableValue(fadeValueAtMouseDown, effectBlock->fadeIn->floatValue());
		resized();
	}
	else if (e.eventComponent == &fadeOutHandle)
	{
		effectBlock->fadeOut->setUndoableValue(fadeValueAtMouseDown, effectBlock->fadeOut->floatValue());
		resized();
	}
}

void EffectBlockUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	LayerBlockUI::controllableFeedbackUpdateInternal(c);

	if (c == effectBlock->fadeIn) fadeInHandle.setCentrePosition((effectBlock->fadeIn->floatValue() / effectBlock->getTotalLength()) * getWidth(), fadeInHandle.getHeight() / 2);
	else if (c == effectBlock->fadeOut) fadeOutHandle.setCentrePosition((1 - effectBlock->fadeOut->floatValue() / effectBlock->getTotalLength()) * getWidth(), fadeOutHandle.getHeight() / 2);
}

void EffectBlockUI::newMessage(const EffectBlock::EffectBlockEvent& e)
{
	switch (e.type)
	{
	case EffectBlock::EffectBlockEvent::FADES_CHANGED:
		fadeInHandle.setVisible(effectBlock->fadeIn->enabled);
		fadeOutHandle.setVisible(effectBlock->fadeOut->enabled);
		resized();
		repaint();
		break;

	case EffectBlock::EffectBlockEvent::REGENERATE_PREVIEW:
		break;
	}
}

EffectBlockFadeHandle::EffectBlockFadeHandle()
{
	setSize(12, 12);
}

void EffectBlockFadeHandle::paint(Graphics& g)
{
	g.setColour(isMouseOverOrDragging() ? HIGHLIGHT_COLOR : NORMAL_COLOR);
	g.fillRoundedRectangle(getLocalBounds().reduced(3).toFloat(), 2);
}
