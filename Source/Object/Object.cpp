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

Object::Object(var params) :
	BaseItem(params.getProperty("name", "Object")),
	objectType(params.getProperty("type", "Object").toString()),
	objectData(params),
	previousID(-1)
{
	saveAndLoadRecursiveData = true;

	editorIsCollapsed = true;

	itemDataType = "Object";

	File iconFile = File(params.getProperty("path", "")).getChildFile("icon.png");
	if (iconFile.existsAsFile()) customThumbnailPath = iconFile;

	globalID = addIntParameter("Global ID", "Virtual ID that is used in many places of Blux to filter, alter effects, etc.", 0, 0);
	stagePosition = addPoint3DParameter("Stage Position", "Position on stage, can be animated with stage layouts");
	excludeFromScenes = addBoolParameter("Exclude From Scenes", "If enabled, this object will not be modified when loading scenes", false);

	targetInterface = addTargetParameter("Interface", "The interface to link this object to", InterfaceManager::getInstance());
	targetInterface->targetType = TargetParameter::CONTAINER;
	targetInterface->maxDefaultSearchLevel = 0;

	if (InterfaceManager::getInstance()->items.size() == 1) targetInterface->setValueFromTarget(InterfaceManager::getInstance()->items[0]);

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


	if (ObjectManager::getInstance()->blackOut->boolValue())
	{
		for (auto& p : c->computedParameters) p->setValue(0);
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

void Object::saveSceneData(var &sceneData)
{
	if (excludeFromScenes->boolValue()) return;
	//sceneData.getDynamicObject()->setProperty(globalID->getControlAddress(), globalID->intValue()); //needs better handling from ObjectManager auto ID stuff
	componentManager.saveSceneData(sceneData);
	effectManager.saveSceneData(sceneData);
}


//void Object::sendComponentParameter(ObjectComponent* c, Parameter* p)
//{
//    if (!enabled->boolValue() || !c->enabled) return;
//    if (Interface* i = dynamic_cast<Interface*>(targetInterface->targetContainer.get()))
//    {
//        i->updateValuesFromParameter(this, c, p, p->floatValue());
//    }
//}
