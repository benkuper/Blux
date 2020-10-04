/*
  ==============================================================================

    ObjectComponentEditor.cpp
    Created: 26 Sep 2020 7:51:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectComponentEditor.h"

ObjectComponentEditor::ObjectComponentEditor(ObjectComponent* component, bool isRoot) :
    BaseItemEditor(component, isRoot),
    component(component)
{
    for (int i = 0; i < component->computedParameters.size(); i++)
    {
        computedUI.add(component->computedParameters[i]->createDefaultUI());
        addAndMakeVisible(computedUI[i]);
    }
}

ObjectComponentEditor::~ObjectComponentEditor()
{
}

void ObjectComponentEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    for (int i = computedUI.size() - 1; i >= 0; i--)
    {
        computedUI[i]->setBounds(r.removeFromRight(100).reduced(2));
    }

}
