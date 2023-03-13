/*
  ==============================================================================

	ObjectComponent.cpp
	Created: 26 Sep 2020 10:03:23am
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"
#include "Interface/InterfaceIncludes.h"

ObjectComponent::ObjectComponent(Object* o, String name, ComponentType componentType, var params) :
	BaseItem(name, true),
	object(o),
	componentType(componentType),
	mainParameter(nullptr),
	interfaceParamCC("Interface Params")
{
	saveAndLoadRecursiveData = true;

	excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked ,this will not be included in scene data", false);
	canBeCopiedAndPasted = false;
	userCanDuplicate = false;
	canBeReorderedInEditor = true;

	interfaceParamCC.hideInEditor = interfaceParams.isEmpty();
	addChildControllableContainer(&interfaceParamCC);
}

ObjectComponent::~ObjectComponent()
{
}

void ObjectComponent::rebuildInterfaceParams(Interface* interface)
{

	var oldData = interfaceParamsGhostData.isVoid() ? interfaceParamCC.getJSONData() : interfaceParamsGhostData;

	interfaceParamCC.clear();
	interfaceParams.clear();
	computedInterfaceMap.clear();

	if (interface == nullptr) return;

	//default behaviours here
	if (DMXInterface* dmxI = dynamic_cast<DMXInterface*>(interface))
	{
		interfaceParamCC.setNiceName("DMX Parameters");

		int i = 1;
		for (auto& cp : computedParameters)
		{
			IntParameter* p = interfaceParamCC.addIntParameter(cp->niceName, "Channel for this parameter", i, 1, 512, true);
			p->canBeDisabledByUser = true;
			interfaceParams.add(p);
			computedInterfaceMap.set(cp, p);
			i += p->value.size();
		}
	}

	interfaceParamCC.hideInEditor = interfaceParams.isEmpty();
	interfaceParamCC.loadJSONData(oldData);

	interfaceParamsGhostData = var();
}

Parameter* ObjectComponent::addComputedParameter(Parameter* p, ControllableContainer* parent)
{
	if (parent == nullptr) parent = this;
	sourceParameters.add(p);
	parent->addParameter(p);

	Parameter* cp = ControllableFactory::createParameterFrom(p, false, true);
	cp->setNiceName("Out " + p->niceName);
	cp->setControllableFeedbackOnly(true);
	cp->hideInEditor = true;
	computedParameters.add(cp);
	computedParamMap.set(cp, p);
	paramComputedMap.set(p, cp);
	addParameter(cp);

	if (computedParameters.size() == 1) mainParameter = cp;

	return p;
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
		if (computedParamMap.contains(c))
		{
			//DBG("Set computed value to source value " << computedParamMap[c]->niceName << " : " << computedParamMap[c]->floatValue());
			values.set(c, computedParamMap[c]->getValue().clone());
		}
	}
}

void ObjectComponent::updateComputedValues(HashMap<Parameter*, var>& values)
{
	for (auto& p : computedParameters)
	{
		//DBG("update computed value after chain, " << p->niceName << " : " << values[p].toString());
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
		var channelsData = data.getProperty("channels", var());

		for (auto& cp : computedParameters)
		{
			Parameter* channelP = computedInterfaceMap[cp];

			if (channelP == nullptr) continue;
			int channel = channelP->intValue();
			int targetChannel = channelOffset + channel - 1; //convert local channel to 0-based

			if (cp->isComplex())
			{
				var val = cp->getValue();
				for (int i = 0; i < val.size(); i++)
				{
					if (cp->type == Controllable::FLOAT) channelsData[targetChannel + i] = (int)((float)val[i] * 255);
					else if (cp->type == Controllable::INT) channelsData[targetChannel + i] = (int)val[i];
				}
			}
			else
			{
				if (cp->type == Controllable::FLOAT) channelsData[targetChannel] = (int)(cp->floatValue() * 255);
				else if (cp->type == Controllable::INT) channelsData[targetChannel] = cp->intValue();
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

var ObjectComponent::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("interfaceParams", interfaceParamCC.getJSONData());
	return data;
}

void ObjectComponent::loadJSONDataItemInternal(var data)
{
	interfaceParamsGhostData = data.getProperty("interfaceParams", var());
}

InspectableEditor* ObjectComponent::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ObjectComponentEditor(this, isRoot);
}
