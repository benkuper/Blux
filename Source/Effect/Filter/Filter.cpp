/*
  ==============================================================================

    Filter.cpp
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "Filter.h"
#include "Object/Object.h"

#include "ui/FilterEditor.h"
#include "Common/Helpers/SceneHelpers.h"


Filter::Filter(const String& name) :
    BaseItem(name)
{
    itemDataType = "Filter";
    saveAndLoadRecursiveData = true;
    useLocalID = addBoolParameter("Use Local ID", "If enabled, this will use the generated ID from this filter instead of the object's global ID", false);

    excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked, this will not be saved in scene", false);

    showInspectorOnSelect = false;

}

Filter::~Filter()
{
}

FilterResult Filter::getFilteredResultForComponent(Object* o, ObjectComponent* c)
{
    if (!enabled->boolValue()) return FilterResult();
    
    FilterResult r = getFilteredResultForComponentInternal(o, c);
    if (r.id == -1) return r;

    return FilterResult({ useLocalID->boolValue() ? r.id : o->globalID->intValue(), r.weight });
}

FilterResult Filter::getFilteredResultForComponentInternal(Object* o, ObjectComponent* c)
{
    return FilterResult({ o->globalID->intValue(), 1 });
}

var Filter::getSceneData()
{
    return SceneHelpers::getParamsSceneData(this, { excludeFromScenes });
;}

void Filter::updateSceneData(var& sceneData)
{
}

void Filter::lerpFromSceneData(var startData, var endData, float weight)
{
    if (excludeFromScenes->boolValue()) return;
    SceneHelpers::lerpSceneParams(this, startData, endData, weight);
}
