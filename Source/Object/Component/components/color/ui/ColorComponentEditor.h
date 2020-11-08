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
#include "Color/ColorSource/ui/ColorSourceViz.h"
#include "Color/ColorSource/ui/ColorSourceChooser.h"

class ColorComponentEditor :
    public ObjectComponentEditor,
    public ColorSourceChooser::ChooserListener
{
public:
    ColorComponentEditor(ColorComponent* comp, bool isRoot);
    ~ColorComponentEditor();

    ColorComponent* comp;
    ColorSourceChooser chooser;

    ColorSourceViz* viz;

    void resetAndBuild() override;
    void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

    void sourceChosen(const String& type, ColorSource* templateRef) override;
};