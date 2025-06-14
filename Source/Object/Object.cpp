/*
  ==============================================================================

	Object.cpp
	Created: 26 Sep 2020 10:02:32am
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"
#include "Interface/InterfaceIncludes.h"
#include "Effect/EffectIncludes.h"
#include "Scene/SceneIncludes.h"
#include "Sequence/SequenceIncludes.h"

Object::Object(var params) :
	BaseItem(params.getProperty("name", "Object")),
	objectType(params.getProperty("type", "Object").toString()),
	objectData(params),
	previousID(-1),
	slideManipParameter(nullptr)
{
	setHasCustomColor(true);
	showWarningInUI = true;
	saveAndLoadRecursiveData = true;

	//hideInEditor = true;
	editorIsCollapsed = true;
	hideInEditor = true;

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

	icon->addOption("Custom", -1);

	customIcon = addFileParameter("Custom Icon", "Custom icon for this object");
	customIcon->fileTypeFilter = "*.jpg;*.png;*.jpeg;*.bmp";
	customIcon->setEnabled(false);

	globalID = addIntParameter("Global ID", "Virtual ID that is used in many places of Blux to filter, alter effects, etc.", 0, 0);
	stagePosition = addPoint3DParameter("Stage Position", "Position on stage, can be animated with stage layouts");
	stageRotation = addPoint3DParameter("Stage Rotation", "Rotation on stage, can be animated with stage layouts");
	excludeFromScenes = addBoolParameter("Exclude From Scenes", "If enabled, this object will not be modified when loading scenes", false);

	targetInterface = addTargetParameter("Interface", "The interface to link this object to", InterfaceManager::getInstance());
	targetInterface->targetType = TargetParameter::CONTAINER;
	targetInterface->maxDefaultSearchLevel = 0;

	componentManager.reset(new ComponentManager(this));

	var cData = params.getProperty("components", var());
	if (cData.isObject())
	{
		NamedValueSet cProps = cData.getDynamicObject()->getProperties();
		for (auto& cp : cProps) componentManager->addComponentFromDefinition(cp.name, cp.value, false);
	}

	customParams.reset(new ObjectManagerCustomParams(ObjectManager::getInstance()));
	addChildControllableContainer(customParams.get());

	//componentManager->userCanAddItemsManually = params.getProperty("isCustom", ftalse);
	componentManager->addManagerListener(this);
	componentsChanged();//force one time here to set things from components

	addChildControllableContainer(componentManager.get());

	effectManager.reset(new EffectManager());
	addChildControllableContainer(effectManager.get());

	//bool canCustomize = params.getProperty("canCustomize", false);
	//var objectsData = params.getProperty("objects", var());

	/*if (objectsData.isObject() || canCustomize)
	{
		objectManager.reset(new SubObjectManager());
		objectManager->userCanAddItemsManually = canCustomize;
		addChildControllableContainer(objectManager.get());
	}*/

	if (!Engine::mainEngine->isLoadingFile && InterfaceManager::getInstance()->items.size() == 1) targetInterface->setValueFromTarget(InterfaceManager::getInstance()->items[0]);
}

Object::~Object()
{
}

void Object::clearItem()
{
	effectManager->clear();
	componentManager->removeManagerListener(this);
	if (!sourceInterfaceParamsRef.wasObjectDeleted() && sourceInterfaceParamsRef != nullptr)
	{
		sourceInterfaceParamsRef->removeControllableContainerListener(this);
	}

}

void Object::rebuildInterfaceParams()
{
	if (!sourceInterfaceParamsRef.wasObjectDeleted() && sourceInterfaceParamsRef != nullptr)
	{
		sourceInterfaceParamsRef->removeControllableContainerListener(this);
	}

	if (interfaceParameters != nullptr)
	{
		var gData = interfaceParameters->getJSONData();
		if (!gData.isVoid()) interfaceGhostData = gData;
		removeChildControllableContainer(interfaceParameters.get());
	}

	sourceInterfaceParamsRef = nullptr;
	if (targetInterface->targetContainer != nullptr)
	{
		sourceInterfaceParamsRef = targetInterface->targetContainer;
		sourceInterfaceParamsRef->addControllableContainerListener(this);

		interfaceParameters.reset(((Interface*)targetInterface->targetContainer.get())->getInterfaceParams());
		addChildControllableContainer(interfaceParameters.get(), false, 0);

	}
	else interfaceParameters.reset();

	if (interfaceParameters != nullptr && !interfaceGhostData.isVoid()) interfaceParameters->loadJSONData(interfaceGhostData);

	if (componentManager != nullptr) for (auto& c : componentManager->items) c->rebuildInterfaceParams((Interface*)targetInterface->targetContainer.get());

	if (DimmerComponent* ic = getComponent<DimmerComponent>()) slideManipParameter = ic->value;
	else slideManipParameter = nullptr;
}

ObjectComponent* Object::getComponentForType(ComponentType t)
{
	for(auto & c: componentManager->items) if(c->componentType == t) return c;
	return nullptr;
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
		if (!isCurrentlyLoadingData) stagePosition->setVector(viewUIPosition->x, stagePosition->y, viewUIPosition->y);
	}
	else if (p == icon)
	{
		var iv = icon->getValueData();
		customIcon->setEnabled(iv.isInt() && (int)iv == -1);
	}
}

void Object::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	if (cc == sourceInterfaceParamsRef)
	{
		if (Parameter* p = dynamic_cast<Parameter*>(c))
		{
			if (Parameter* pp = interfaceParameters->getParameterByName(p->shortName))
			{
				pp->setRange(p->minimumValue, p->maximumValue);
			}
		}
	}
}



void Object::checkAndComputeComponentValuesIfNeeded()
{
	if (!enabled->boolValue() || Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing) return;

	for (auto& c : componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;
		//if (c->isDirty || effectManager.items.size() > 0 || GlobalEffectManager::getInstance()->items.size() > 0);
		 //to do here, implement a cleaner way to know if an object should recompute (effects may not need constant recompute, and are not targetting all components.)
		computeComponentValues(c); //right now, always compute
	}

	if (Interface* i = dynamic_cast<Interface*>(targetInterface->targetContainer.get()))
	{
		i->sendValuesForObject(this);
	}
}

void Object::computeComponentValues(ObjectComponent* c)
{
	if (!c->enabled->boolValue()) return;

	c->update();
	HashMap<Parameter*, var> values;
	c->fillComputedValueMap(values);

	if (values.size() > 0)
	{
		if (!ObjectManager::getInstance()->blackOut->boolValue())
		{

			//local effects
			effectManager->processComponent(this, c, values);

			//scene effects
			SceneManager::getInstance()->processComponent(this, c, values);

			//group effects
			GroupManager::getInstance()->processComponent(this, c, values); //to optimize with group registration on add/remove and not checking always at compute time

			//global effects
			GlobalSequenceManager::getInstance()->processComponent(this, c, values);
			GlobalEffectManager::getInstance()->processComponent(this, c, values);


		}

		c->updateComputedValues(values);
	}

}

var Object::getSceneData()
{
	if (excludeFromScenes->boolValue()) return var(new DynamicObject());
	var data(new DynamicObject());
	data.getDynamicObject()->setProperty(componentManager->shortName, componentManager->getSceneData());
	data.getDynamicObject()->setProperty(effectManager->shortName, effectManager->getSceneData());
	return data;
}

void Object::updateSceneData(var& sceneData)
{
}

void Object::lerpFromSceneData(var startData, var endData, float weight)
{
	if (excludeFromScenes->boolValue()) return;
	componentManager->lerpFromSceneData(startData.getProperty(componentManager->shortName, var()), endData.getProperty(componentManager->shortName, var()), weight);
	effectManager->lerpFromSceneData(startData.getProperty(effectManager->shortName, var()), endData.getProperty(effectManager->shortName, var()), weight);
}

var Object::getVizData()
{
	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("niceName", niceName);
	data.getDynamicObject()->setProperty("controlAddress", getControlAddress(Engine::mainEngine));
	data.getDynamicObject()->setProperty("stagePosition", stagePosition->getValue());
	data.getDynamicObject()->setProperty("stageRotation", stageRotation->getValue());
	data.getDynamicObject()->setProperty("type", getTypeString());
	data.getDynamicObject()->setProperty("shape", "Moving Head");
	data.getDynamicObject()->setProperty("components", componentManager->getVizData());
	return data;
}

void Object::componentsChanged()
{
	if (DimmerComponent* ic = getComponent<DimmerComponent>()) slideManipParameter = ic->value;
	else slideManipParameter = nullptr;

	if (!isCurrentlyLoadingData)
	{
		if (Interface* i = (Interface*)targetInterface->targetContainer.get()) for (auto& c : componentManager->items) c->rebuildInterfaceParams(i);
	}
}

void Object::afterLoadJSONDataInternal()
{
	if (Interface* i = (Interface*)targetInterface->targetContainer.get()) for (auto& c : componentManager->items) c->rebuildInterfaceParams(i);
}

Array<ChainVizTarget*> Object::getEffectChain(ComponentType ct)
{
	Array<ChainVizTarget*> result;
	result.addArray(effectManager->getChainVizTargetsForObjectAndComponent(this, ct));
	result.addArray(SceneManager::getInstance()->getChainVizTargetsForObjectAndComponent(this, ct));
	result.addArray(GroupManager::getInstance()->getChainVizTargetsForObjectAndComponent(this, ct));
	//result.addArray(GlobalSequenceManager::getInstance()->.getChainVizTargetsForObjectAndComponent(this, ct));
	result.addArray(GlobalEffectManager::getInstance()->getChainVizTargetsForObjectAndComponent(this, ct));
	return result;
}

ChainVizComponent* Object::createVizComponent(Object* o, ComponentType ct, ChainVizTarget::ChainVizType type)
{
	jassert(o == this);
	return new ObjectChainVizUI(this, ct, type);
}