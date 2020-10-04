/*
  ==============================================================================

    FilterManagerEditor.cpp
    Created: 26 Sep 2020 7:50:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "FilterManagerEditor.h"

FilterManagerEditor::FilterManagerEditor(FilterManager* manager, bool isRoot) :
    GenericManagerEditor(manager, isRoot),
    componentSelectorUI(&manager->componentSelector)
{
    addAndMakeVisible(&componentSelectorUI);
}

FilterManagerEditor::~FilterManagerEditor()
{
}

void FilterManagerEditor::resizedInternalHeader(Rectangle<int>& r)
{
    GenericManagerEditor::resizedInternalHeader(r);
    componentSelectorUI.setBounds(r.removeFromRight(100).reduced(2));

}
