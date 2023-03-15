/*
  ==============================================================================

	Effect.cpp
	Created: 30 Sep 2020 12:38:01pm
	Author:  bkupe

  ==============================================================================
*/

#include "EffectIncludes.h"
#include "Effect.h"

Effect::Effect(const String& name, var params) :
	BaseItem(name),
	forceDisabled(false),
	parentGroup(nullptr),
	idMode(nullptr),
	computePreviousValues(false),
	effectParams("Effect Parameters")
{
	//effectParams.hideEditorHeader = true;
	//effectParams.editorCanBeCollapsed = false;

	effectParams.addParamLinkContainerListener(this);

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

	addChildControllableContainer(&effectParams);

	filterManager.reset(new FilterManager());
	addChildControllableContainer(filterManager.get());
	//showInspectorOnSelect = false;

	canBeCopiedAndPasted = true;
	
	var typesF = params.getProperty("types",var());
	if (typesF.isArray())
	{
		for (int i = 0; i < typesF.size(); i++) typeFilters.add((ComponentType)(int)typesF[i]);
	}
	else if(!typesF.isVoid())
	{
		typeFilters.add((ComponentType)(int)typesF);
	}
}

Effect::~Effect()
{
}

bool Effect::isAffectingObject(Object* o)
{
	return filterManager->isAffectingObject(o);
}

void Effect::setParentGroup(Group* g)
{
	parentGroup = g;
	if (parentGroup != nullptr)
	{
		idMode = addEnumParameter("ID Mode", "Chooses how to handle re-identification depending on the filtered content. This will override filtered IDs", true);
		idMode->addOption("Do not change", NO_CHANGE)->addOption("Use local", LOCAL)->addOption("Use local reversed", LOCAL_REVERSE)->addOption("Use local randomized", RANDOMIZED);
		controllables.move(controllables.indexOf(idMode), controllables.indexOf(sceneSaveMode));
	}
}

void Effect::setForceDisabled(bool value)
{
	if (forceDisabled == value) return;
	forceDisabled = value;
	updateEnabled();
}

void Effect::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier, int id, float time)
{
	if (!typeFilters.isEmpty() && !typeFilters.contains(c->componentType)) return;

	FilterResult r = filterManager->getFilteredResultForComponent(o, c);
	if (r.id == -1)
	{
		if (c->componentType == ComponentType::DIMMER) o->effectIntensityOutMap.set(this, ((DimmerComponent*)c)->value->floatValue());
		return;
	}

	int targetID = (id != -1 && r.id == o->globalID->intValue()) ? id : r.id;

	if (idMode != nullptr)
	{
		IDMode m = idMode->getValueDataAsEnum<IDMode>();
		int localID = parentGroup->getLocalIDForObject(o);
		if (m == LOCAL) targetID = localID;
		if (m == LOCAL_REVERSE) targetID = parentGroup->getNumObjects() - 1 - localID;
		else if (m == RANDOMIZED) targetID = parentGroup->getRandomIDForObject(o);
	}

	float targetWeight = r.weight * weight->floatValue() * weightMultiplier;

	if (targetWeight == 0)
	{
		if (c->componentType == ComponentType::DIMMER) o->effectIntensityOutMap.set(this, ((DimmerComponent*)c)->value->floatValue());
		return;
	}

	if (computePreviousValues)
	{
		if (!prevValuesMap.contains(c))
		{
			HashMap<Parameter*, var>* prevVals = new HashMap<Parameter*, var>();
			HashMap<Parameter*, var>::Iterator it(values);
			while (it.next()) prevVals->set(it.getKey(), it.getValue().clone());
			prevValues.add(prevVals);
			prevValuesMap.set(c, prevVals);
		}
	}

	HashMap<Parameter*, var> targetValues;
	processComponentInternal(o, c, values, targetValues, targetID, time);

	HashMap<Parameter*, var>::Iterator it(targetValues);
	while (it.next())
	{
		Parameter* cp = it.getKey();
		var initVal = values[cp];
		var val = it.getValue().clone();
		if (initVal != val)
		{
			var bVal = blendValue(initVal, val, targetWeight);
			values.set(cp, bVal);
		}
	}

	if (computePreviousValues)
	{
		HashMap<Parameter*, var>* prevVals = prevValuesMap[c];
		HashMap<Parameter*, var>::Iterator it(targetValues);
		while (it.next())
		{
			DBG("Set prev value " << it.getKey()->niceName << " : " << it.getValue().toString());
			prevVals->set(it.getKey(), it.getValue().clone());
		}
	}
}


void Effect::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		updateEnabled();
		if (enabled->boolValue()) clearPrevValues();
	}
	else if (p == weight && weight->floatValue() == 0) clearPrevValues();
}

void Effect::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	if (cc == &effectParams) effectParamChanged(c);
}

void Effect::paramControlModeChanged(ParamLinkContainer* pc, ParameterLink* pl)
{
	effectListeners.call(&EffectListener::effectParamControlModeChanged, pl->parameter);
}

void Effect::clearPrevValues()
{
	prevValuesMap.clear();
	prevValues.clear();
}

void Effect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{

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
		for (int i = 0; i < start.size(); i++)
		{
			if (start[i].isArray())
			{
				var v;
				for (int j = 0; j < start[i].size(); j++) v.append(blendFloatValue(start[i][j], end[i][j], weight));
				result.append(v);
			}
			else
			{
				result.append(blendFloatValue(start[i], end[i], weight));
			}
		}
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
	float targetVal = 0;
	switch (blendMode)
	{
	case OVERRIDE: targetVal = end; break;
	case ADD: targetVal = start + end; break;
	case MAX: targetVal = jmax(start, end); break;
	case MIN: targetVal = jmin(start, end); break;
	case MULTIPLY: targetVal = start * end; break;
	}

	float val = jmap<float>(weight, start, targetVal);
	return  val;
}

var Effect::getSceneData()
{
	SceneSaveMode m = sceneSaveMode->getValueDataAsEnum<SceneSaveMode>();
	if (m == NONE) return var(new DynamicObject());
	else if (m == FULL)
	{
		var data = SceneHelpers::getParamsSceneData(this, Array<Parameter*>());
		data.getDynamicObject()->setProperty(effectParams.shortName, SceneHelpers::getParamsSceneData(&effectParams, Array<Parameter*>(), true));
		data.getDynamicObject()->setProperty(filterManager->shortName, filterManager->getSceneData());
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
	else if (m == FULL)
	{
		SceneHelpers::lerpSceneParams(this, startData, endData, lerpWeight);
		filterManager->lerpFromSceneData(startData.getProperty(filterManager->shortName, var()), endData.getProperty(filterManager->shortName, var()), lerpWeight);
		SceneHelpers::lerpSceneParams(&effectParams, startData.getProperty(effectParams.shortName, var()), endData.getProperty(effectParams.shortName, var()), lerpWeight, true);
	}
	else if (m == WEIGHT_ONLY) SceneHelpers::lerpSceneParam(weight, startData, endData, lerpWeight);
}

ChainVizComponent* Effect::createVizComponent(Object* o, ChainVizTarget::ChainVizType type)
{
	return new EffectChainVizUI(this, o, type);
}

InspectableEditor* Effect::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new EffectEditor(this, isRoot);
}