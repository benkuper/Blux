/*
  ==============================================================================

	ObjectComponent.cpp
	Created: 26 Sep 2020 10:03:23am
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"
#include "Interface/InterfaceIncludes.h"
#include "ObjectComponent.h"

ObjectComponent::ObjectComponent(Object* o, String name, ComponentType componentType, var params) :
	BaseItem(name, true),
	object(o),
	componentType(componentType),
	mainParameter(nullptr),
	interfaceParams("Interface Params")
{
	excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked ,this will not be included in scene data", false);
	canBeCopiedAndPasted = false;
	canBeReorderedInEditor = false;

	interfaceParams.hideInEditor = interfaceParams.controllables.isEmpty();
}

ObjectComponent::~ObjectComponent()
{
}

void ObjectComponent::generateInterfaceParams(Interface* i)
{
	interfaceParams.clear();
	if (i == nullptr) return;

	//default behaviours here

	interfaceParams.hideInEditor = interfaceParams.controllables.isEmpty();
}

void ObjectComponent::addComputedParameter(Parameter* p, Parameter* originalParameter)
{
	p->setControllableFeedbackOnly(true);
	computedParameters.add(p);
	computedParamMap.set(p, originalParameter);
	addParameter(p);

	if (computedParameters.size() == 1) mainParameter = p;

	p->hideInEditor = true;
}

void ObjectComponent::linkComputedParamToDMXChannel(Parameter* cp, int localChannel)
{
	localDMXChannels.set(cp, localChannel);
}

void ObjectComponent::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);

	if (!enabled->boolValue()) return;

}

void ObjectComponent::fillComputedValueMap(HashMap<Parameter*, var>& values)
{
	for (auto& c : computedParameters)
	{
		if (computedParamMap.contains(c)) values.set(c, computedParamMap[c]->getValue().clone());
	}
}

void ObjectComponent::updateComputedValues(HashMap<Parameter*, var>& values)
{
	for (auto& p : computedParameters)
	{
		p->setValue(values[p]);
	}
}

void ObjectComponent::setupFromJSONDefinition(var data)
{
	//interfaceParams.loadJSONData(data.getProperty("interf")) = (int)data.getProperty("channel", 1) - 1; //-1 because it's an offset and definitions are defining with first channel = 1
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

void ObjectComponent::fillInterfaceData(Interface* i, var data, var params)
{
	if (i == nullptr) return;
	if (!enabled->boolValue()) return;
	if (!data.isObject()) return;

	fillInterfaceDataInternal(i, data, params);
}

void ObjectComponent::fillInterfaceDataInternal(Interface* i, var data, var params)
{
	//depending on interface, change what's happening here.

	if (DMXInterface* di = dynamic_cast<DMXInterface*>(i))
	{
		int channelOffset = params.getProperty("channelOffset", 0);
		HashMap<Parameter*, int>::Iterator it(localDMXChannels);
		var channelsData = data.getProperty("channels", var());
		while (it.next())
		{
			int targetChannel = channelOffset + it.getValue() - 1;
			Parameter* p = it.getKey();
			if (p->isComplex())
			{
				var val = p->getValue();
				for (int i = 0; i < val.size(); i++)
				{
					if (p->type == Controllable::FLOAT) channelsData[targetChannel + i] = (int)((float)val[i] * 255);
					else if (p->type == Controllable::INT) channelsData[targetChannel + i] = (int)val[i];
				}
			}
			else
			{
				if (p->type == Controllable::FLOAT) channelsData[targetChannel] = (int)(it.getKey()->floatValue() * 255);
				else if (p->type == Controllable::INT) channelsData[targetChannel] = it.getKey()->intValue();
			}
		}
	}
	else
	{
		DynamicObject* valData = data.getProperty("values", var()).getDynamicObject();
		if (valData == nullptr) return;

		var cData(new DynamicObject());
		for (auto& p : computedParameters) cData.getDynamicObject()->setProperty(p->shortName, p->getValue());
		valData->setProperty(shortName, cData);
	}
}

//void ObjectComponent::fillOutValueMap(HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset)
//{
//	int sChannel = startChannel + (ignoreChannelOffset ? 0 : channelOffset);
//
//	for (int i = 0; i < computedParameters.size(); i++)
//	{
//		Parameter* p = computedParameters[i];
//		if (p->isComplex())
//		{
//			for (int j = 0; j < p->value.size(); j++) channelValueMap.set(sChannel + paramChannels[i] + j, (float)p->value[j]); //remap to 0-255 automatically
//		}
//		else channelValueMap.set(sChannel + paramChannels[i], p->floatValue()); //remap to 0-255 automatically
//	}
//}

InspectableEditor* ObjectComponent::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ObjectComponentEditor(this, isRoot);
}
