/*
  ==============================================================================

    ObjectComponentEditor.h
    Created: 26 Sep 2020 7:51:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ObjectComponentEditor :
    public BaseItemEditor
{
public:
    ObjectComponentEditor(ObjectComponent * component, bool isRoot, bool showComputedParams = true);
    virtual ~ObjectComponentEditor();

    ObjectComponent* component;

    bool showComputedParams;
    OwnedArray<ControllableUI> computedUI;

    virtual void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
};