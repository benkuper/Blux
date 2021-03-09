/*
  ==============================================================================

    ColorComponentEditor.cpp
    Created: 2 Nov 2020 7:35:32pm
    Author:  bkupe

  ==============================================================================
*/

#include "ColorComponentEditor.h"

ColorComponentEditor::ColorComponentEditor(ColorComponent* comp, bool isRoot) :
    ObjectComponentEditor(comp, isRoot, false),
    comp(comp)
{
    colorChooser.addChooserListener(this);
    shapeChooser.addChooserListener(this);

    colorChooser.setButtonText(comp->colorSource != nullptr ? comp->colorSource->getSourceLabel() : "No Color Source");
    shapeChooser.setButtonText(comp->pixelShape != nullptr ? comp->pixelShape->getTypeString() : "No Shape");

    addAndMakeVisible(&colorChooser);
    addAndMakeVisible(&shapeChooser);

    comp->addAsyncColorComponentListener(this);
}

ColorComponentEditor::~ColorComponentEditor()
{
    colorChooser.removeChooserListener(this);
    shapeChooser.removeChooserListener(this);
    if(!inspectable.wasObjectDeleted()) comp->removeAsyncColorComponentListener(this);
}


void ColorComponentEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    colorChooser.setBounds(r.removeFromRight(100).reduced(2));
    r.removeFromRight(2);
    shapeChooser.setBounds(r.removeFromRight(100).reduced(2));
    r.removeFromRight(2);
}

void ColorComponentEditor::sourceChosen(const String& type, ColorSource* templateRef)
{
    comp->setupSource(type, templateRef);
}

void ColorComponentEditor::shapeChosen(const String& type)
{
    comp->setupShape(type);
}

void ColorComponentEditor::newMessage(const ColorComponent::ColorComponentEvent& e)
{
    if (inspectable.wasObjectDeleted() || comp->isClearing) return;

    switch (e.type)
    {
    case ColorComponent::ColorComponentEvent::SOURCE_CHANGED:
    {
        colorChooser.setButtonText(comp->colorSource != nullptr ? comp->colorSource->getSourceLabel() : "No Color Source");
    }
    break;

    case ColorComponent::ColorComponentEvent::SHAPE_CHANGED:
    {
        shapeChooser.setButtonText(comp->pixelShape != nullptr ? comp->pixelShape->getTypeString() : "No Shape");
    }
    break;
    }
}
