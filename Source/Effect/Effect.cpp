/*
  ==============================================================================

	Effect.cpp
	Created: 30 Sep 2020 12:38:01pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect.h"
#include "ui/EffectEditor.h"
#include "Common/CommonIncludes.h"
#include "Object/Object.h"
#include "ui/EffectChainVizUI.h"

Effect::Effect(const String& name, var params) :
	BaseItem(name),
	forceDisabled(false)
{
	saveAndLoadRecursiveData = true;

	sceneSaveMode = addEnumParameter("Save Mode", "Choose what to save in scenes");
	sceneSaveMode->addOption("Save all", FULL, false)->addOption("Save weight only", WEIGHT_ONLY)->addOption("Exclude", NONE);
	sceneSaveMode->setValueWithData(WEIGHT_ONLY);
	
	mode = addEnumParameter("Blend Mode", "Defines how the values are blended with the source ones.");
	mode->addOption("Override", OVERRIDE)->addOption("Max", MAX)->addOption("Min", MIN)->addOption("Add", ADD)->addOption("Multiply", MULTIPLY);

	enabled->setValue(false); //This is to avoid dangerous object manipulation on effect creation

	weight = addFloatParameter("Weight", "Weight of this effect", 1, 0, 1);
	weight->defaultValue = 0; //this allows for scene lerp default to 0
	weight->hideInEditor = true;

	
	//excludeFromScenes->hideInEditor = true;

	addChildControllableContainer(&filterManager);
	//showInspectorOnSelect = false;

	canBeCopiedAndPasted = true;
}

Effect::~Effect()
{
}

bool Effect::isAffectingObject(Object* o)
{
	return filterManager.isAffectingObject(o);
}

void Effect::setForceDisabled(bool value)
{
	if (forceDisabled == value) return;
	forceDisabled = value;
	updateEnabled();
}

void Effect::processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier, int id, float time)
{
	FilterResult r = filterManager.getFilteredResultForComponent(o, c);
	if (r.id == -1)
	{
		if (c->componentType == ComponentType::INTENSITY && values.size() > 0) o->effectIntensityOutMap.set(this, values[0]);
		return;
	}

	int targetID = (id != -1 && r.id == o->globalID->intValue()) ? id : r.id;
	float targetWeight = r.weight * weight->floatValue() * weightMultiplier;

	if (targetWeight == 0)
	{
		if (c->componentType == ComponentType::INTENSITY && values.size() > 0) o->effectIntensityOutMap.set(this, values[0]);
		return;
	}

	var pValues = getProcessedComponentValuesInternal(o, c,  values.clone(), targetID, time);
	jassert(pValues.size() == values.size());

	for (int i = 0; i < values.size(); i++)
	{
		/*if (values[i].isArray())
		{
			jassert(pValues[i].isArray());
			for (int j = 0; j < values[i].size(); j++)
			{
				var value = pValues[i][j];
				
				values[i][j] = blendFloatValue(values[i][j], pValues[i][j], targetWeight);
			}
		}
		else
		{
		*/
			values[i] = blendValue(values[i], pValues[i], targetWeight);
		//}
	}
}


void Effect::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == enabled) updateEnabled();
}

var Effect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c,var values, int id, float time)
{
	return values;
}

bool Effect::isFullyEnabled()
{
	return enabled->boolValue() && !forceDisabled;
}

var Effect::blendValue(var start, var end, float weight)
{
	jassert(start.size() == end.size());
	var result;
	if (start.isArray())
	{
		for (int i = 0; i < start.size(); i++) result.append(blendFloatValue(start[i], end[i], weight));
	}
	else
	{
		result = blendFloatValue(start, end, weight);
	}

	return result;
}

float Effect::blendFloatValue(float start, float end, float weight)
{
	BlendMode blendMode = mode->getValueDataAsEnum<BlendMode>();
	float val = jmap<float>(weight, start, end);
	switch (blendMode)
	{
	case OVERRIDE: return val;
	case ADD: return start + val;
	case MAX: return jmax(start, val);
	case MIN: return jmin(start, val);
	case MULTIPLY: return start * val;
	}

	return  val;
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

ChainVizComponent* Effect::createVizComponent(Object* o, ChainVizTarget::ChainVizType type)
{
	return new EffectChainVizUI(this, o, type);
}

InspectableEditor* Effect::getEditor(bool isRoot)
{
	return new EffectEditor(this, isRoot);
}