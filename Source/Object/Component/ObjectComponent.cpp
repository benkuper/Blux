/*
  ==============================================================================

	ObjectComponent.cpp
	Created: 26 Sep 2020 10:03:23am
	Author:  bkupe

  ==============================================================================
*/


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

void ObjectComponent::setupFromJSONDefinition(var data)
{
	channelOffset = (int)data.getProperty("channel", 1) - 1; //-1 because it's an offset and definitions are defining with first channel = 1
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

void ObjectComponent::fillOutValueMap(HashMap<int, float>& channelValueMap, int startChannel)
{
	int sChannel = startChannel + channelOffset;

	for (int i = 0; i < computedParameters.size(); i++)
	{
		Parameter* p = computedParameters[i];
		if (p->isComplex())
		{
			for (int j = 0; j < p->value.size(); j++) channelValueMap.set(sChannel + paramChannels[i] + j, (float)p->value[j] * 255); //remap to 0-255 automatically
		}
		else channelValueMap.set(sChannel + paramChannels[i], p->floatValue()); //remap to 0-255 automatically
	}
}

InspectableEditor* ObjectComponent::getEditorInternal(bool isRoot)
{
	return new ObjectComponentEditor(this, isRoot);
}
