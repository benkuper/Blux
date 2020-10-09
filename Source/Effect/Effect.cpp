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

	enabled->setValue(false); //This is to avoid dangerous object manipulation on effect creation

	weight = addFloatParameter("Weight", "Weight of this effect", 1, 0, 1);
	weight->defaultValue = 0; //this allows for scene lerp default to 0
	weight->hideInEditor = true;

	sceneSaveMode = addEnumParameter("Save Mode", "Choose what to save in scenes");
	sceneSaveMode->addOption("Save all", FULL, false)->addOption("Save weight only", WEIGHT_ONLY)->addOption("Exclude", NONE);
	sceneSaveMode->setValueWithData(WEIGHT_ONLY);

	//excludeFromScenes->hideInEditor = true;

	addChildControllableContainer(&filterManager);
	showInspectorOnSelect = false;

	canBeCopiedAndPasted = true;
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
	jassert(pValues.size() == values.size());
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
	SceneSaveMode m = sceneSaveMode->getValueDataAsEnum<SceneSaveMode>();
	if (m == NONE) return var(new DynamicObject());
	else if (m == FULL)
	{
		var data = SceneHelpers::getParamsSceneData(this);
		data.getDynamicObject()->setProperty("filters", filterManager.getSceneData());
		return data;
	}

	//WEIGHT_ONLY
	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("weight", weight->floatValue());
	return data;
}

void Effect::updateSceneData(var& sceneData)
{
}

void Effect::lerpFromSceneData(var startData, var endData, float lerpWeight)
{
	SceneSaveMode m = sceneSaveMode->getValueDataAsEnum<SceneSaveMode>();
	if (m == NONE) return;
	else if (m == FULL) SceneHelpers::lerpSceneParams(this, startData, endData, lerpWeight);
	else if (m == WEIGHT_ONLY) SceneHelpers::lerpSceneParam(weight, startData, endData, lerpWeight);
}

InspectableEditor* Effect::getEditor(bool isRoot)
{
	return new EffectEditor(this, isRoot);
}