/*
  ==============================================================================

    FilterManager.h
    Created: 26 Sep 2020 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Filter.h"

class FilterManager :
    public BaseManager<Filter>
{
public:
    FilterManager();
    ~FilterManager();

    Factory<Filter> factory;
    ComponentSelector componentSelector;

    void saveSceneData(var &sceneData);
    int getFilteredIDForComponent(Object* o, ObjectComponent * c);

    InspectableEditor* getEditor(bool isRoot) override;
};