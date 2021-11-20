#include "CustomOSCInterface.h"
/*
  ==============================================================================

	CustomOSCInterface.cpp
	Created: 18 Nov 2021 3:03:41pm
	Author:  bkupe

  ==============================================================================
*/


CustomOSCInterface::CustomOSCInterface() :
	OSCInterface(getTypeString(), true),
	customParams("Custom Parameters", false, false, false, false)
{
	addChildControllableContainer(&customParams);
	customParams.addBaseManagerListener(this);
}

CustomOSCInterface::~CustomOSCInterface()
{
}

void CustomOSCInterface::itemAdded(GenericControllableItem*)
{
	customOSCListeners.call(&CustomOSCInterfaceListener::customParamsChanged, this);
}

void CustomOSCInterface::itemRemoved(GenericControllableItem*)
{
	customOSCListeners.call(&CustomOSCInterfaceListener::customParamsChanged, this);
}

void CustomOSCInterface::sendValuesForObject(Object* o)
{
	Array<var> args;
	args.add(o->getScriptObject());

	var valuesData(new DynamicObject());
	for (auto& c : o->componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;

		var compData(new DynamicObject());
		for (auto& p : c->computedParameters) compData.getDynamicObject()->setProperty(p->shortName, p->getValue());
		valuesData.getDynamicObject()->setProperty(c->shortName, compData);
	}
	args.add(valuesData);
	
	var itfParams = ((CustomOSCParams*)o->interfaceParameters.get())->getParamValues();
	args.add(itfParams);

	scriptManager->callFunctionOnAllItems("sendValuesForObject", args);
}

CustomOSCInterface::CustomOSCParams::CustomOSCParams(CustomOSCInterface* i) :
	ControllableContainer("Interface Parameters"),
	itf(i)
{
	if (itf != nullptr) itf->addCustomOSCInterfaceListener(this);
	rebuildArgsFromInterface();
}


CustomOSCInterface::CustomOSCParams::~CustomOSCParams()
{
	if (itf != nullptr) itf->removeCustomOSCInterfaceListener(this);
	itf = nullptr;
}


void CustomOSCInterface::CustomOSCParams::customParamsChanged(CustomOSCInterface*)
{
	rebuildArgsFromInterface();
}

void CustomOSCInterface::CustomOSCParams::rebuildArgsFromInterface()
{
	if (itf == nullptr) return;

	var oldData = getJSONData();
	clear();

	for (auto& gci : itf->customParams.items)
	{
		if (gci->controllable->type == Controllable::TRIGGER) continue;

		if (Parameter* p = ControllableFactory::createParameterFrom((Parameter *)gci->controllable, true, true))
		{
			p->canBeDisabledByUser = true;
			p->setEnabled(false);
			addParameter(p);
		}
	}

	loadJSONData(oldData);
}

var CustomOSCInterface::CustomOSCParams::getParamValues()
{
	Array<WeakReference<Parameter>> params = getAllParameters();

	var values(new DynamicObject());
	for (auto& p : params)
	{
		Parameter* targetP = p;
		if (!p->enabled)
		{
			if (GenericControllableItem* gci = itf->customParams.getItemWithName(p->shortName)) targetP = (Parameter*)gci->controllable;
			else targetP = nullptr;
		}

		if (targetP != nullptr) values.getDynamicObject()->setProperty(targetP->shortName, targetP->getValue());

	}
	return values;
}
