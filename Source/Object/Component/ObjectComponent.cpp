/*
  ==============================================================================

	ObjectComponent.cpp
	Created: 26 Sep 2020 10:03:23am
	Author:  bkupe

  ==============================================================================
*/

#include "ObjectComponent.h"
#include "ui/ObjectComponentEditor.h"
#include "Common/Helpers/SceneHelpers.h" 
#include "../Object.h"

const String ObjectComponent::typeNames[TYPES_MAX]{ "Intensity", "Color", "Gobo", "Laser", "PanTilt", "Servo", "Stepper", "Strobe", "Script", "Custom" };

ObjectComponent::ObjectComponent(Object* o, String name, ComponentType componentType, var params) :
	BaseItem(name),
	object(o),
	componentType(componentType),
	isDirty(true)
{
	excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked ,this will not be included in scene data", false);
	canBeCopiedAndPasted = false;
	canBeReorderedInEditor = false;
}

ObjectComponent::~ObjectComponent()
{
}

void ObjectComponent::addComputedParameter(Parameter* p, int channel, Parameter* originalParameter)
{
	p->setControllableFeedbackOnly(true);
	computedParameters.add(p);
	computedParamMap.set(p, originalParameter);
	addParameter(p);
	p->hideInEditor = true;
	paramChannels.add(channel);
}

void ObjectComponent::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);

	if (p == enabled) isDirty = true;
	if (!enabled->boolValue()) return;

	isDirty = true;
}

var ObjectComponent::getOriginalComputedValues()
{
	var values;
	for (auto& c : computedParameters)
	{
		if (computedParamMap.contains(c)) values.append(computedParamMap[c]->getValue().clone());
		else values.append(var());
	}
	return values;
}

var ObjectComponent::getSceneData()
{
	if (excludeFromScenes->boolValue()) return var(new DynamicObject());
	return SceneHelpers::getParamsSceneData(this, { excludeFromScenes });
}

void ObjectComponent::updateSceneData(var& sceneData)
{
}

void ObjectComponent::lerpFromSceneData(var startData, var endData, float weight)
{
	if (excludeFromScenes->boolValue()) return;
	SceneHelpers::lerpSceneParams(this, startData, endData, weight);
}

InspectableEditor* ObjectComponent::getEditor(bool isRoot)
{
	return new ObjectComponentEditor(this, isRoot);
}
