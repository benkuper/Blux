/*
  ==============================================================================

	RawDataBlockUI.cpp
	Created: 13 Dec 2022 6:24:21pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"


RawDataBlockUI::RawDataBlockUI(RawDataBlock* b) :
	LayerBlockUI(b),
	rawDataBlock(b),
	fadeValueAtMouseDown(0)
{
	bgColor = BG_COLOR.darker(.4f);

	addChildComponent(&fadeInHandle, 0);
	addChildComponent(&fadeOutHandle, 0);

	fadeInHandle.setVisible(rawDataBlock->fadeIn->enabled);
	fadeOutHandle.setVisible(rawDataBlock->fadeOut->enabled);

	fadeInHandle.addMouseListener(this, false);
	fadeOutHandle.addMouseListener(this, false);

	rawDataBlock->addAsyncRawDataBlockListener(this);
}

RawDataBlockUI::~RawDataBlockUI()
{
	if (!inspectable.wasObjectDeleted()) rawDataBlock->removeAsyncRawDataBlockListener(this);
}


void RawDataBlockUI::paint(Graphics& g)
{
	LayerBlockUI::paint(g);

	g.setColour(bgColor.brighter());
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 1);

	g.setColour(TEXT_COLOR);
	g.setFont(g.getCurrentFont().withHeight(jlimit<float>(10, 20, getHeight() - 20)).boldened());
	g.drawText(rawDataBlock->niceName, getCoreBounds().toFloat(), Justification::centred);
}

void RawDataBlockUI::paintOverChildren(Graphics& g)
{
	LayerBlockUI::paintOverChildren(g);

	Colour fInColor = (rawDataBlock->fadeIn->enabled ? NORMAL_COLOR : BLUE_COLOR).withAlpha(.5f);
	Colour fOutColor = (rawDataBlock->fadeOut->enabled ? NORMAL_COLOR : BLUE_COLOR).withAlpha(.5f);

	float lastFrameTime = rawDataBlock->getLastFrameTime();
	if (lastFrameTime != rawDataBlock->coreLength->floatValue())
	{
		float rel = rawDataBlock->coreLength->floatValue() / lastFrameTime;
		if (rel > 1) rel = 1 / rel;
		Rectangle<int> r = getCoreBounds().withLeft(rel * getCoreWidth());
		g.setColour(Colours::white.withAlpha(.1f));
		g.fillRoundedRectangle(r.toFloat(), 2.0f);
	}

	if (rawDataBlock->fadeIn->floatValue() > 0)
	{
		g.setColour(fInColor);
		g.drawLine(0, getHeight(), getWidth() * (rawDataBlock->fadeIn->floatValue() / rawDataBlock->getTotalLength()), fadeInHandle.getY() + fadeInHandle.getHeight() / 2);
	}

	if (rawDataBlock->fadeOut->floatValue() > 0)
	{
		g.setColour(fOutColor);
		g.drawLine(getWidth() * (1 - (rawDataBlock->fadeOut->floatValue() / rawDataBlock->getTotalLength())), fadeOutHandle.getY() + fadeOutHandle.getHeight() / 2, getWidth(), getHeight());
	}
}

void RawDataBlockUI::resizedBlockInternal()
{


	fadeInHandle.setCentrePosition((rawDataBlock->fadeIn->floatValue() / rawDataBlock->getTotalLength()) * getWidth(), fadeInHandle.getHeight() / 2);
	fadeOutHandle.setCentrePosition((1 - rawDataBlock->fadeOut->floatValue() / rawDataBlock->getTotalLength()) * getWidth(), fadeOutHandle.getHeight() / 2);
}


void RawDataBlockUI::mouseDown(const MouseEvent& e)
{
	LayerBlockUI::mouseDown(e);

	if (e.eventComponent == &fadeInHandle) fadeValueAtMouseDown = rawDataBlock->fadeIn->floatValue();
	else if (e.eventComponent == &fadeOutHandle) fadeValueAtMouseDown = rawDataBlock->fadeOut->floatValue();
}

void RawDataBlockUI::mouseDrag(const MouseEvent& e)
{
	LayerBlockUI::mouseDrag(e);

	if (e.eventComponent == &fadeInHandle)
	{
		rawDataBlock->fadeIn->setValue((getMouseXYRelative().x * 1.0f / getWidth()) * rawDataBlock->getTotalLength());
		resized();
	}
	else if (e.eventComponent == &fadeOutHandle)
	{
		rawDataBlock->fadeOut->setValue((1 - (getMouseXYRelative().x * 1.0f / getWidth())) * rawDataBlock->getTotalLength());
		resized();
	}
}

void RawDataBlockUI::mouseUp(const MouseEvent& e)
{
	LayerBlockUI::mouseUp(e);

	if (e.eventComponent == &fadeInHandle)
	{
		rawDataBlock->fadeIn->setUndoableValue(fadeValueAtMouseDown, rawDataBlock->fadeIn->floatValue());
		resized();
	}
	else if (e.eventComponent == &fadeOutHandle)
	{
		rawDataBlock->fadeOut->setUndoableValue(fadeValueAtMouseDown, rawDataBlock->fadeOut->floatValue());
		resized();
	}
}

void RawDataBlockUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	LayerBlockUI::controllableFeedbackUpdateInternal(c);

	if (c == rawDataBlock->fadeIn || c == rawDataBlock->fadeOut)
	{
		if (c == rawDataBlock->fadeIn) fadeInHandle.setCentrePosition((rawDataBlock->fadeIn->floatValue() / rawDataBlock->getTotalLength()) * getWidth(), fadeInHandle.getHeight() / 2);
		else if (c == rawDataBlock->fadeOut) fadeOutHandle.setCentrePosition((1 - rawDataBlock->fadeOut->floatValue() / rawDataBlock->getTotalLength()) * getWidth(), fadeOutHandle.getHeight() / 2);

		repaint();
	}

}

void RawDataBlockUI::newMessage(const RawDataBlock::RawDataBlockEvent& e)
{
	switch (e.type)
	{
	case RawDataBlock::RawDataBlockEvent::LOADED:
		break;

	case RawDataBlock::RawDataBlockEvent::FADES_CHANGED:
		fadeInHandle.setVisible(rawDataBlock->fadeIn->enabled);
		fadeOutHandle.setVisible(rawDataBlock->fadeOut->enabled);
		resized();
		repaint();
		break;
	}
}