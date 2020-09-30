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
    if (component->computedParameters.size() > 0)
    {
        computedUI.reset(component->computedParameters[0]->createDefaultUI());
        addAndMakeVisible(computedUI.get());
    }
}

ObjectComponentEditor::~ObjectComponentEditor()
{
}

void ObjectComponentEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    if (computedUI != nullptr)
    {
        computedUI->setBounds(r.removeFromRight(150).reduced(2));
    }

}
