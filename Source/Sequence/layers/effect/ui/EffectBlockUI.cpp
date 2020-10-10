/*
  ==============================================================================

	EffectBlockUI.cpp
	Created: 10 Oct 2020 10:47:39am
	Author:  bkupe

  ==============================================================================
*/

#include "EffectBlockUI.h"

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
	fadeInHandle.setCentrePosition((effectBlock->fadeIn->floatValue() / effectBlock->getTotalLength()) * getWidth(), fadeInHandle.getHeight() / 2);
	fadeOutHandle.setCentrePosition((1 - effectBlock->fadeOut->floatValue() / effectBlock->getTotalLength()) * getWidth(), fadeOutHandle.getHeight() / 2);
}


void EffectBlockUI::mouseDown(const MouseEvent& e)
{
	LayerBlockUI::mouseDown(e);

	if (e.eventComponent == &fadeInHandle) fadeValueAtMouseDown = effectBlock->fadeIn->floatValue();
	else if (e.eventComponent == &fadeOutHandle) fadeValueAtMouseDown = effectBlock->fadeOut->floatValue();
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
	setSize(14, 14);
}

void EffectBlockFadeHandle::paint(Graphics& g)
{
	g.setColour(isMouseOverOrDragging() ? HIGHLIGHT_COLOR : NORMAL_COLOR);
	g.fillRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 2);
}
