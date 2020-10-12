/*
  ==============================================================================

	ObjectAction.cpp
	Created: 8 Oct 2020 3:31:13pm
	Author:  bkupe

  ==============================================================================
*/

#include "ObjectAction.h"
#include "Object/ObjectManager.h"

ObjectAction::ObjectAction(var params) :
	Action(params)
{
	target = addTargetParameter("Target", "Target to set the value", ObjectManager::getInstance());

	actionType = (ActionType)(int)params.getProperty("actionType", SET_COMPONENT_PARAMETER_VALUE);

	if (actionType == SET_COMPONENT_PARAMETER_VALUE)
	{
		target->targetType = TargetParameter::CONTROLLABLE;
		target->customGetTargetFunc = &ObjectAction::showAndGetComponentParameter;
	}
	else
	{
		target->targetType = TargetParameter::CONTAINER;
		target->maxDefaultSearchLevel = 0;
	}
}


ObjectAction::~ObjectAction()
{
}

void ObjectAction::setValueParameter(Parameter* p)
{
	if (Engine::mainEngine->isLoadingFile) return;

	if (!value.wasObjectDeleted() && value != nullptr)
	{
		ghostValueData = value->getJSONData();
		removeControllable(value.get());
	}

	Parameter* tp = dynamic_cast<Parameter*>(target->target.get());
	if (tp == nullptr) return;

	value = p;

	if (value != nullptr)
	{
		addParameter(p);
		if (!ghostValueData.isVoid()) value->loadJSONData(ghostValueData);
		ghostValueData = var();
	}
}


void ObjectAction::triggerInternal()
{
	if (target->target == nullptr) return;

	switch (actionType)
	{
	case SET_COMPONENT_PARAMETER_VALUE:
	{
		if (value == nullptr) return;
		Parameter* p = static_cast<Parameter*>(target->target.get());
		p->setValue(value->getValue());
	}
	break;

	case ENABLE_OBJECT:
	case DISABLE_OBJECT:
		if (Object* o = dynamic_cast<Object*>(target->targetContainer.get()))
		{
			o->enabled->setValue(actionType == ENABLE_OBJECT);
		}
		break;
		break;
	}
}

void ObjectAction::setValueInternal(var _value)
{
	switch (actionType)
	{
	case SET_COMPONENT_PARAMETER_VALUE:
	{
		Parameter* p = static_cast<Parameter*>(target->target.get());

		if (!value->isComplex() || value->value.size() == _value.size())
		{
			value->setValue(_value);
			p->setValue(value->getValue());
		}
	}
	break;

	case ENABLE_OBJECT:
	case DISABLE_OBJECT:
		if (Object* o = dynamic_cast<Object*>(target->targetContainer.get()))
		{
			o->enabled->setValue((float)_value == 1);
		}
		break;
	}

}

void ObjectAction::onContainerParameterChanged(Parameter* p)
{
	if (p == target)
	{
		if (actionType == SET_COMPONENT_PARAMETER_VALUE)
		{
			if (!Engine::mainEngine->isLoadingFile)
			{

				if (target->target == nullptr) setValueParameter(nullptr);
				else
				{
					if (target->target->type == Controllable::TRIGGER) setValueParameter(nullptr);
					else
					{
						Controllable* c = ControllableFactory::createParameterFrom(target->target);
						if (c == nullptr)
						{
							DBG("Should not be null here");
							jassertfalse;
						}

						c->setNiceName("Value");
						Parameter* tp = dynamic_cast<Parameter*>(c);
						setValueParameter(tp);
					}
				}
			}
		}

	}
}

Controllable* ObjectAction::showAndGetComponentParameter(bool showParameters, bool showTriggers)
{
	Array<Parameter*> params;
	PopupMenu m;
	int index = 1;
	for (auto& o : ObjectManager::getInstance()->items)
	{
		PopupMenu om;
		for (auto& c : o->componentManager.items)
		{
			PopupMenu cm;
			for (auto& p : c->sourceParameters)
			{
				params.add(p);
				cm.addItem(index++, p->niceName);
			}

			om.addSubMenu(c->niceName, cm);
		}

		m.addSubMenu(o->niceName, om);
	}

	int result = m.show();

	if (result >= 1) return params[result - 1];

	return nullptr;
}

void ObjectAction::loadJSONDataInternal(var data)
{
	if (Engine::mainEngine->isLoadingFile)
	{
		//DBG("Engine is loading, waiting after load");
		Engine::mainEngine->addEngineListener(this);
		dataToLoad = data;
	}
	else Action::loadJSONDataInternal(data);
}

void ObjectAction::endLoadFile()
{
	//reset data we want to reload
	target->setValue("", true);
	//DBG("Engine after load, load command data");

	loadJSONData(dataToLoad);
	dataToLoad = var();

	Engine::mainEngine->removeEngineListener(this);
}