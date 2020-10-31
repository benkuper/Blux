/*
  ==============================================================================

	Object.cpp
	Created: 26 Sep 2020 10:02:32am
	Author:  bkupe

  ==============================================================================
*/

#include "Object.h"
#include "Interface/InterfaceManager.h"
#include "Object/Component/components/intensity/IntensityComponent.h"
#include "Effect/GlobalEffectManager.h"
#include "Group/GroupManager.h"
#include "Scene/SceneManager.h"
#include "Object/ObjectManager.h"
#include "Sequence/GlobalSequenceManager.h"
#include "ObjectManager.h"
#include "ui/ObjectChainVizUI.h"

Object::Object(var params) :
	BaseItem(params.getProperty("name", "Object")),
	objectType(params.getProperty("type", "Object").toString()),
	objectData(params),
	previousID(-1),
	slideManipParameter(nullptr)
{
	saveAndLoadRecursiveData = true;

	editorIsCollapsed = true;

	itemDataType = "Object";

	File objPath = File(params.getProperty("path", ""));

	icon = addEnumParameter("Icon", "Bring some fancy in you life ! Put icons in the objects folders and find them here to customize you view");
	
	if (objPath.exists())
	{

		File defaultON = objPath.getChildFile("icon_on.png");
		File defaultOFF = objPath.getChildFile("icon_off.png");
		File defaultImg = objPath.getChildFile("icon.png");

		var defaultOpt;

		if (defaultON.existsAsFile() && defaultOFF.existsAsFile())
		{
			defaultOpt.append(defaultOFF.getFullPathName());
			defaultOpt.append(defaultON.getFullPathName());
		}
		else
		{
			defaultOpt = defaultImg.getFullPathName();
		}

		icon->addOption("Default", defaultOpt);

		File staticFolder = objPath.getChildFile("icons/static");
		if (staticFolder.isDirectory())
		{
			Array<File> files = staticFolder.findChildFiles(File::findFiles, false, "*.png");
			for (auto& f : files)
			{
				icon->addOption(f.getFileNameWithoutExtension(), f.getFullPathName());
			}
		}

		File variableFolder = objPath.getChildFile("icons/variable");
		if (variableFolder.isDirectory())
		{
			Array<File> files = variableFolder.findChildFiles(File::findFiles, false, "*_on.png");
			for (auto& f : files)
			{
				File offFile = variableFolder.getChildFile(f.getFileName().replace("_on", "_off"));
				if (offFile.existsAsFile())
				{
					String base = f.getFileNameWithoutExtension();
					String fName = base.substring(0, base.length() - 3);
					var opt;
					opt.append(offFile.getFullPathName());
					opt.append(f.getFullPathName());
					icon->addOption(fName, opt);
				}
			}
		}
	}

	globalID = addIntParameter("Global ID", "Virtual ID that is used in many places of Blux to filter, alter effects, etc.", 0, 0);
	stagePosition = addPoint3DParameter("Stage Position", "Position on stage, can be animated with stage layouts");
	excludeFromScenes = addBoolParameter("Exclude From Scenes", "If enabled, this object will not be modified when loading scenes", false);

	targetInterface = addTargetParameter("Interface", "The interface to link this object to", InterfaceManager::getInstance());
	targetInterface->targetType = TargetParameter::CONTAINER;
	targetInterface->maxDefaultSearchLevel = 0;

	if (!Engine::mainEngine->isLoadingFile && InterfaceManager::getInstance()->items.size() == 1) targetInterface->setValueFromTarget(InterfaceManager::getInstance()->items[0]);

	var cData = params.getProperty("components", var());
	if (cData.isObject())
	{
		NamedValueSet cProps = cData.getDynamicObject()->getProperties();
		for (auto& cp : cProps) componentManager.addComponentFromDefinition(cp.name, cp.value, false);
	}

	if (IntensityComponent* ic = getComponent<IntensityComponent>())	slideManipParameter = ic->values[0];

	componentManager.userCanAddItemsManually = params.getProperty("isCustom", false);
	addChildControllableContainer(&componentManager);

	addChildControllableContainer(&effectManager);

	bool canCustomize = params.getProperty("canCustomize", false);
	var objectsData = params.getProperty("objects", var());

	if (objectsData.isObject() || canCustomize)
	{
		objectManager.reset(new SubObjectManager());
		objectManager->userCanAddItemsManually = canCustomize;
		addChildControllableContainer(objectManager.get());
	}
}

Object::~Object()
{
}

void Object::rebuildInterfaceParams()
{
	if (interfaceParameters != nullptr)
	{
		var gData = interfaceParameters->getJSONData();
		if (!gData.isVoid()) interfaceGhostData = gData;
		removeChildControllableContainer(interfaceParameters.get());
	}

	if (targetInterface->targetContainer != nullptr)
	{
		interfaceParameters.reset(((Interface*)targetInterface->targetContainer.get())->getInterfaceParams());
		addChildControllableContainer(interfaceParameters.get(), false, 0);
	}
	else interfaceParameters.reset();

	if (interfaceParameters != nullptr && !interfaceGhostData.isVoid()) interfaceParameters->loadJSONData(interfaceGhostData);
}

void Object::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == targetInterface)
	{
		rebuildInterfaceParams();
	}
	else if (p == globalID)
	{
		objectListeners.call(&ObjectListener::objectIDChanged, this, previousID);
		previousID = globalID->intValue();
	}
	else if (p == stagePosition)
	{
		viewUIPosition->setPoint(stagePosition->x, stagePosition->z);
	}
	else if (p == viewUIPosition)
	{
		if(!isCurrentlyLoadingData) stagePosition->setVector(viewUIPosition->x, stagePosition->y, viewUIPosition->y);
	}
}

void Object::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	BaseItem::onControllableFeedbackUpdateInternal(cc, c);

	if (!enabled->boolValue()) return;

	//if (c->type != Parameter::TRIGGER)
	//{
	//    if (ObjectComponent* oc = c->getParentAs<ObjectComponent>())
	//    {
	//        if (targetInterface->targetContainer != nullptr)
	//        {
	//            if (!oc->computedParameters.contains((Parameter*)c)) computeComponentValues(oc);
	//            //else sendComponentParameter(oc, ((Parameter*)c));
	//        }
	//    }
	//}
}


void Object::checkAndComputeComponentValuesIfNeeded()
{
	if (!enabled->boolValue()) return;

	for (auto& c : componentManager.items)
	{
		if (!c->enabled->boolValue()) continue;
		//if (c->isDirty || effectManager.items.size() > 0 || GlobalEffectManager::getInstance()->items.size() > 0);
		 //to do here, implement a cleaner way to know if an object should recompute (effects may not need constant recompute, and are not targetting all components.)
		computeComponentValues(c); //right now, always compute
	}
}

void Object::computeComponentValues(ObjectComponent* c)
{
	if (!enabled->boolValue() || !c->enabled->boolValue() || Engine::mainEngine->isClearing) return;

	effectIntensityOutMap.clear();

	if (ObjectManager::getInstance()->blackOut->boolValue())
	{
		for (auto& p : c->computedParameters)
		{
			var zeroVal;
			if (p->isComplex()) for (int i = 0; i < p->value.size(); i++) zeroVal.append(0);
			else zeroVal = 0;
			p->setValue(zeroVal);
		}
	}
	else
	{
		var values = c->getOriginalComputedValues();
		if (!values.isVoid())
		{
			//local effects
			effectManager.processComponentValues(this, c, values);

			//scene effects
			SceneManager::getInstance()->processComponentValues(this, c, values);

			//group effects
			GroupManager::getInstance()->processComponentValues(this, c, values); //to optimize with group registration on add/remove and not checking always at compute time

			//global effects
			GlobalSequenceManager::getInstance()->processComponentValues(this, c, values);


			GlobalEffectManager::getInstance()->processComponentValues(this, c, values);

			int index = 0;
			for (auto& p : c->computedParameters)
			{
				p->setValue(values[index++]);
			}
		}
	}

	c->isDirty = false;

	if (Interface* i = dynamic_cast<Interface*>(targetInterface->targetContainer.get()))
	{
		i->updateValuesFromComponent(this, c);
	}
}

var Object::getSceneData()
{
	if (excludeFromScenes->boolValue()) return var(new DynamicObject());
	var data(new DynamicObject());
	data.getDynamicObject()->setProperty(componentManager.shortName, componentManager.getSceneData());
	data.getDynamicObject()->setProperty(effectManager.shortName,effectManager.getSceneData());
	return data;
}

void Object::updateSceneData(var& sceneData)
{
}

void Object::lerpFromSceneData(var startData, var endData, float weight)
{
	if (excludeFromScenes->boolValue()) return;
	componentManager.lerpFromSceneData(startData.getProperty(componentManager.shortName, var()), endData.getProperty(componentManager.shortName, var()), weight);
	effectManager.lerpFromSceneData(startData.getProperty(effectManager.shortName, var()), endData.getProperty(effectManager.shortName,var()), weight);
}

Array<ChainVizTarget *> Object::getEffectChain()
{
	Array<ChainVizTarget *> result;
	result.addArray(effectManager.getChainVizTargetsForObject(this));
	result.addArray(SceneManager::getInstance()->getChainVizTargetsForObject(this));
	result.addArray(GroupManager::getInstance()->getChainVizTargetsForObject(this));
	//result.addArray(GlobalSequenceManager::getInstance()->.getChainVizTargetsForObject(this));
	result.addArray(GlobalEffectManager::getInstance()->getChainVizTargetsForObject(this));
	return result;
}

ChainVizComponent* Object::createVizComponent(Object * o, ChainVizTarget::ChainVizType type)
{
	jassert(o == this);
	return new ObjectChainVizUI(this,  type);
}