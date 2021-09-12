/*
  ==============================================================================

    Filter.cpp
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

Filter::Filter(const String& name) :
    BaseItem(name)
{
    itemDataType = "Filter";
    saveAndLoadRecursiveData = true;
    idMode = addEnumParameter("ID Moed", "Chooses how to handle re-identification depending on the filtered content", true);
    idMode->addOption("Do not change", NO_CHANGE, false)->addOption("Use local", LOCAL)->addOption("Use local reversed", LOCAL_REVERSE)->addOption("Use local randomized", RANDOMIZED);
    idMode->setValueWithData(LOCAL);

    excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked, this will not be saved in scene", false);
    excludeFromScenes->hideInEditor = true;

    showInspectorOnSelect = false;

}

Filter::~Filter()
{
}

bool Filter::isAffectingObject(Object* o)
{
    return true;
}

FilterResult Filter::getFilteredResultForComponent(Object* o, ObjectComponent* c)
{
    if (!enabled->boolValue()) return FilterResult();

    FilterResult r = getFilteredResultForComponentInternal(o, c);
    if (r.id == -1) return r;

    IDMode m = idMode->getValueDataAsEnum<IDMode>();
    return FilterResult({ m != NO_CHANGE ? r.id : o->globalID->intValue(), r.weight });
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
