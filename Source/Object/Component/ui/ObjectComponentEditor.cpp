/*
  ==============================================================================

    ObjectComponentEditor.cpp
    Created: 26 Sep 2020 7:51:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

ObjectComponentEditor::ObjectComponentEditor(ObjectComponent* component, bool isRoot, bool showComputedParams) :
    BaseItemEditor(component, isRoot),
    component(component),
    showComputedParams(showComputedParams)
{
    if (showComputedParams)
    {
        if (component->mainParameter != nullptr)
        {
            computedUI.add(component->mainParameter->createDefaultUI());
            addAndMakeVisible(computedUI[0]);
        }
        else
        {
            for (int i = 0; i < component->computedParameters.size(); i++)
            {
                computedUI.add(component->computedParameters[i]->createDefaultUI());
                addAndMakeVisible(computedUI[i]);
            }
        }

    }

    for (auto& cui : computedUI) cui->showLabel = !((Parameter*)cui->controllable.get())->isComplex();
}

ObjectComponentEditor::~ObjectComponentEditor()
{
}

void ObjectComponentEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    if (showComputedParams)
    {
        for (int i = computedUI.size() - 1; i >= 0; i--)
        {
            computedUI[i]->setBounds(r.removeFromRight(200).reduced(2));
        }
    }
}
