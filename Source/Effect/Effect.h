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

    enum BlendMode { OVERRIDE, ADD, MAX, MIN, MULTIPLY };
    EnumParameter* mode;

    FloatParameter* weight;

    enum SceneSaveMode { FULL, WEIGHT_ONLY, NONE };
    EnumParameter * sceneSaveMode;

    FilterManager filterManager;

    void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f, int id = -1, float time = -1);
    virtual var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1);
    virtual float blendFloatValue(float start, float end, float weight);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    virtual InspectableEditor* getEditor(bool isRoot) override;
    String getTypeString() const override { return "Effect"; }
};