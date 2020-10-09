/*
  ==============================================================================

    Effect.h
    Created: 30 Sep 2020 12:38:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Filter/FilterManager.h"
class ObjectComponent;
class Object;

class Effect :
    public BaseItem
{
public:
    Effect(const String& name = "Effect", var params = var());
    ~Effect();

    FloatParameter* weight;
    BoolParameter* excludeFromScenes;
    FilterManager filterManager;

    void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f);
    virtual var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    virtual InspectableEditor* getEditor(bool isRoot) override;
    String getTypeString() const override { return "Effect"; }
};