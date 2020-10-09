/*
  ==============================================================================

    Filter.h
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Object/Component/ObjectComponent.h"
#include "Common/ComponentSelector/ComponentSelector.h"

class Object;

struct FilterResult
{
    int id = -1;
    float weight = 1;
};

class Filter :
    public BaseItem
{
public:
    Filter(const String &name = "Filter");
    virtual ~Filter();

    BoolParameter* useLocalID;
    BoolParameter* excludeFromScenes;

    FilterResult getFilteredResultForComponent(Object* o, ObjectComponent * c);
    virtual FilterResult getFilteredResultForComponentInternal(Object* o, ObjectComponent* c);
    
    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);
};