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
	GenericScopedLock lock(interfaceParamsLock);

	var oldData = interfaceParamsGhostData.isVoid() ? interfaceParamCC.getJSONData() : interfaceParamsGhostData;

	interfaceParamCC.clear();
	interfaceParams.clear();
	computedInterfaceMap.clear();

	if (interface == nullptr) return;

	//default behaviours here
	if (DMXInterface* dmxI = dynamic_cast<DMXInterface*>(interface))
	{
		interfaceParamCC.setNiceName("DMX Parameters");

		int i = componentParams.getProperty("channel", 1);
		for (auto& cp : computedParameters)
		{
			IntParameter* p = interfaceParamCC.addIntParameter(cp->niceName + " Channel", "Channel for this parameter", 1, 1, 512, true);
			p->setValue(i, false, true);
			p->canBeDisabledByUser = true;
			if (!checkDefaultInterfaceParamEnabled(computedParamMap[cp])) p->setEnabled(false);
			else  i += p->isComplex() ? p->value.size() : 1;

			interfaceParams.add(p);
			computedInterfaceMap.set(cp, p);
		}
	}

	interfaceParamCC.loadJSONData(oldData);
	interfaceParamCC.hideInEditor = interfaceParams.isEmpty();

	interfaceParamsGhostData = var();
}

Parameter* ObjectComponent::addComputedParameter(Parameter* p, ControllableContainer* parent, bool addToSceneParams)
{
	GenericScopedLock lock(interfaceParamsLock);

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

	if (addToSceneParams) sceneDataParameters.addIfNotAlreadyThere(p);

	if (computedParameters.size() == 1) mainParameter = cp;

	return p;
}

void ObjectComponent::removeComputedParameter(Parameter* p)
{
	GenericScopedLock lock(interfaceParamsLock);

	if (p == nullptr) return;
	if (!computedParameters.contains(p)) return;


	computedParameters.removeAllInstancesOf(p);

	Parameter* sourceP = computedParamMap[p];
	if (sourceP != nullptr)
	{
		sourceParameters.removeAllInstancesOf(sourceP);
		paramComputedMap.remove(sourceP);
		sceneDataParameters.removeAllInstancesOf(sourceP);
		sourceP->parentContainer->removeControllable(sourceP);
	}

	computedParamMap.remove(p);
	p->parentContainer->removeControllable(p);
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
	if (ObjectManager::getInstance()->blackOut->boolValue())
	{
		for (auto& p : computedParameters)
		{
			if (values[p].isArray())
			{
				for (int i = 0; i < values[p].size(); i++)
				{
					if (values[p][i].isArray())
					{
						for (int j = 0; j < values[p].size(); j++)
						{
							values[p][i][j] = 0;
						}
					}
					else
					{
						values[p][i] = 0;
					}
				}
			}
			else
			{
				values[p] = 0;
			}
		}
	}

	for (auto& p : computedParameters)
	{
		//DBG("update computed value after chain, " << p->niceName << " : " << values[p].toString());
		p->setValue(values[p]);
	}
}

void ObjectComponent::setupFromJSONDefinition(var data)
{
	componentParams = data;
}

var ObjectComponent::getSceneData()
{
	if (excludeFromScenes->boolValue()) return var(new DynamicObject());

	var data(new DynamicObject());
	for (auto& p : sceneDataParameters)
	{
		if (p == nullptr || p.wasObjectDeleted()) continue;
		if (p->isControllableFeedbackOnly) continue;
		data.getDynamicObject()->setProperty(p->getControlAddress(this), p->getValue());
	}

	return data;

	//return SceneHelpers::getParamsSceneData(this, { excludeFromScenes });
}

void ObjectComponent::updateSceneData(var& sceneData)
{
}

void ObjectComponent::lerpFromSceneData(var startData, var endData, float weight)
{
	if (excludeFromScenes->boolValue()) return;

	for (auto& p : sceneDataParameters)
	{
		if (p == nullptr || p.wasObjectDeleted()) continue;
		if (p->isControllableFeedbackOnly) continue;

		SceneHelpers::lerpSceneParam(this, p, startData, endData, weight);
		//data.getDynamicObject()->setProperty(p->getControlAddress(this), p->getValue());
	}

	//SceneHelpers::lerpSceneParams(this, startData, endData, weight);
}

var ObjectComponent::getVizData()
{
	var data(new DynamicObject());
	var computedData(new DynamicObject());
	for (auto& p : computedParameters) computedData.getDynamicObject()->setProperty(p->shortName, p->getValue());
	data.getDynamicObject()->setProperty("computedParams", computedData);

	return data;
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
	GenericScopedLock lock(interfaceParamsLock);

	bool blackout = ObjectManager::getInstance()->blackOut->boolValue();

	if (DMXInterface* di = dynamic_cast<DMXInterface*>(i))
	{
		int channelOffset = params.getProperty("channelOffset", 0);
		var channelsData = data.getProperty("channels", var());

		for (auto& cp : computedParameters)
		{
			Parameter* channelP = computedInterfaceMap[cp];
			if (channelP == nullptr || !channelP->enabled) continue;
			int channel = channelP->intValue();
			int targetChannel = channelOffset + channel - 1; //convert local channel to 0-based

			var mappedVal = getMappedValueForComputedParam(i, cp);
			if (cp->isComplex())
			{
				var val = cp->getValue();
				for (int i = 0; i < val.size(); i++) channelsData[targetChannel + i] = blackout ? 0.f : (float)mappedVal[i];
			}
			else
			{
				channelsData[targetChannel] = blackout ? 0.f : (float)mappedVal;
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

var ObjectComponent::getMappedValueForComputedParam(Interface* i, Parameter* cp)
{
	if (DMXInterface* di = dynamic_cast<DMXInterface*>(i))
	{

		if (cp->type == Controllable::FLOAT)  return cp->floatValue() * 255;
		else if (cp->type == Controllable::INT) return cp->intValue();

		if (cp->isComplex())
		{
			var result = cp->getValue().clone();
			for (int i = 0; i < result.size(); i++)
			{
				result[i] = (float)result[i] * 255;
				//result[i] = (int)result[i];
			}

			return result;
		}
	}


	return cp->getValue();
}

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
