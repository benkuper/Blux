/*
  ==============================================================================

    ColorComponentEditor.h
    Created: 2 Nov 2020 7:35:32pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ColorComponent.h"
#include "../../../ui/ObjectComponentEditor.h"
#include "Color/ColorSource/ui/ColorSourceChooser.h"
#include "Color/PixelShape/ui/PixelShapeChooser.h"

class ColorComponentEditor :
    public ObjectComponentEditor,
    public ColorSourceChooser::ChooserListener,
    public PixelShapeChooser::ChooserListener,
    public ColorComponent::AsyncColorComponentListener
{
public:
    ColorComponentEditor(ColorComponent* comp, bool isRoot);
    ~ColorComponentEditor();

    ColorComponent* comp;
    ColorSourceChooser colorChooser;
    PixelShapeChooser shapeChooser;

    void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

    void sourceChosen(const String& type, ColorSource* templateRef) override;
    void shapeChosen(const String& type) override;

    void newMessage(const ColorComponent::ColorComponentEvent& e) override;
};