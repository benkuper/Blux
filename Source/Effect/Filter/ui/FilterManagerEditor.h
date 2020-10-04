/*
  ==============================================================================

    FilterManagerEditor.h
    Created: 26 Sep 2020 7:50:44pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../FilterManager.h"
#include "Common/ComponentSelector/ui/ComponentSelectorUI.h"

class FilterManagerEditor :
    public GenericManagerEditor<Filter>
{
public:
    FilterManagerEditor(FilterManager * manager , bool isRoot);
    ~FilterManagerEditor();

    ComponentSelectorUI componentSelectorUI;

    void resizedInternalHeader(Rectangle<int>& r) override;
};