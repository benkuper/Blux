/*
  ==============================================================================

    FilterManagerEditor.cpp
    Created: 26 Sep 2020 7:50:44pm
    Author:  bkupe

  ==============================================================================
*/

FilterManagerEditor::FilterManagerEditor(FilterManager* manager, bool isRoot) :
    GenericManagerEditor(manager, isRoot)
{
    //if (manager->componentSelector.allowedComponents.size() > 1)
    //{
    //    componentSelectorUI.reset(new ComponentSelectorUI(&manager->componentSelector));
    //    addAndMakeVisible(componentSelectorUI.get());
    //}
}

FilterManagerEditor::~FilterManagerEditor()
{
}

void FilterManagerEditor::resizedInternalHeader(Rectangle<int>& r)
{
    GenericManagerEditor::resizedInternalHeader(r);
    //if(componentSelectorUI != nullptr) componentSelectorUI->setBounds(r.removeFromRight(100).reduced(2));
}
