/*
  ==============================================================================

    ObjectComponentEditor.h
    Created: 26 Sep 2020 7:51:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ObjectComponent.h"

class ObjectComponentEditor :
    public BaseItemEditor
{
public:
    ObjectComponentEditor(ObjectComponent * component, bool isRoot);
    ~ObjectComponentEditor();

    ObjectComponent* component;

    std::unique_ptr<ControllableUI> computedUI;

    void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
};