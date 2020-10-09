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

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    FilterResult getFilteredResultForComponent(Object* o, ObjectComponent * c);

    InspectableEditor* getEditor(bool isRoot) override;

    var getJSONData() override;
    void loadJSONDataManagerInternal(var data) override;
};