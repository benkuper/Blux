/*
  ==============================================================================

    FilterManagerEditor.h
    Created: 26 Sep 2020 7:50:44pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../FilterManager.h"
class ComponentSelectorUI;

class FilterManagerEditor :
    public GenericManagerEditor<Filter>
{
public:
    FilterManagerEditor(FilterManager * manager , bool isRoot);
    ~FilterManagerEditor();

    std::unique_ptr<ComponentSelectorUI> componentSelectorUI;

    void resizedInternalHeader(Rectangle<int>& r) override;
};