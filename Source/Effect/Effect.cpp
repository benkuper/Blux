/*
  ==============================================================================

    Effect.cpp
    Created: 30 Sep 2020 12:38:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "Effect.h"
#include "ui/EffectEditor.h"
#include "Common/Helpers/SceneHelpers.h"

Effect::Effect(const String& name, var 
    params) :
    BaseItem(name)
{
    saveAndLoadRecursiveData = true;

    weight = addFloatParameter("Weight", "Weight of this effect",1,0,1);
    weight->defaultValue = 0; //this allows for scene lerp default to 0
    weight->hideInEditor = true;

    excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked, this effect won't be saved in scens", false);
    //excludeFromScenes->hideInEditor = true;

    addChildControllableContainer(&filterManager);
    showInspectorOnSelect = false;
}

Effect::~Effect()
{
}

void Effect::processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier)
{
    FilterResult r = filterManager.getFilteredResultForComponent(o, c);
    if (r.id == -1) return;

    float targetWeight = r.weight * weight->floatValue() * weightMultiplier;

    if (targetWeight == 0) return;

    var pValues = getProcessedComponentValuesInternal(o, c, r.id, values.clone());
    jassert(pValues.size() == values.size() );
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray())
        {
            jassert(pValues[i].isArray());
            for (int j = 0; j < values[i].size(); j++)
            {
                values[i][j] = jmap<float>(targetWeight, values[i][j], pValues[i][j]);
            }
        }
        else
        {
            values[i] = jmap<float>(targetWeight, values[i], pValues[i]);
        }
    }
}

var Effect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values)
{
    return values;
}

var Effect::getSceneData()
{
    if (excludeFromScenes->boolValue()) return var(new DynamicObject());
    var data = SceneHelpers::getParamsSceneData(this, { excludeFromScenes });
    data.getDynamicObject()->setProperty("filters", filterManager.getSceneData());
    return data;
}

void Effect::updateSceneData(var& sceneData)
{
}

void Effect::lerpFromSceneData(var startData, var endData, float weight)
{
    if (excludeFromScenes->boolValue()) return;
    SceneHelpers::lerpSceneParams(this, startData, endData, weight);
}

InspectableEditor* Effect::getEditor(bool isRoot)
{
    return new EffectEditor(this, isRoot);
}