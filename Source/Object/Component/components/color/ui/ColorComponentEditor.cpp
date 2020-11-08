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
   
    chooser.addChooserListener(this);
    addAndMakeVisible(&chooser);
    
    resetAndBuild();
    resized();
}

ColorComponentEditor::~ColorComponentEditor()
{
    chooser.removeChooserListener(this);
}

void ColorComponentEditor::resetAndBuild()
{
    ObjectComponentEditor::resetAndBuild();
    if (!container->editorIsCollapsed)
    {
        viz = new ColorSourceViz(comp);
        childEditors.add(viz);
        addAndMakeVisible(viz);
    }
    else
    {
        viz = nullptr;
    }
}

void ColorComponentEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    chooser.setBounds(r.removeFromRight(100).reduced(2));
    r.removeFromRight(2);
}

void ColorComponentEditor::sourceChosen(const String& type, ColorSource* templateRef)
{
    comp->setupSource(type, templateRef);
}
