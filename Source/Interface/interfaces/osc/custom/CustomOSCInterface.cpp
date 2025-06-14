/*
  ==============================================================================

	CustomOSCInterface.cpp
	Created: 18 Nov 2021 3:03:41pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface/InterfaceIncludes.h"
#include "Object/ObjectIncludes.h"

CustomOSCInterface::CustomOSCInterface() :
	OSCInterface(getTypeString(), true),
	customParams("Custom Parameters", false, false, false, false)
{
	addChildControllableContainer(&customParams);
	customParams.addManagerListener(this);
}

CustomOSCInterface::~CustomOSCInterface()
{
}

void CustomOSCInterface::itemAdded(GenericControllableItem*)
{
	if (isClearing) return;
	customOSCListeners.call(&CustomOSCInterfaceListener::customParamsChanged, this);
}

void CustomOSCInterface::itemsAdded(Array<GenericControllableItem*>)
{
	if (isClearing) return;
	customOSCListeners.call(&CustomOSCInterfaceListener::customParamsChanged, this);
}

void CustomOSCInterface::itemRemoved(GenericControllableItem*)
{
	if (isClearing) return;
	customOSCListeners.call(&CustomOSCInterfaceListener::customParamsChanged, this);
}

void CustomOSCInterface::itemsRemoved(Array<GenericControllableItem*>)
{
	if (isClearing) return;
	customOSCListeners.call(&CustomOSCInterfaceListener::customParamsChanged, this);
}

void CustomOSCInterface::sendValuesForObjectInternal(Object* o)
{
	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("values", new DynamicObject()); //needed to fill

	for (auto& c : o->componentManager->items)
	{
		if (!c->enabled->boolValue()) continue;
		c->fillInterfaceData(this, data, var());
	}

	var itfParams = ((CustomOSCParams*)o->interfaceParameters.get())->getParamValues();

	Array<var> args;
	args.add(o->getScriptObject());
	args.add(data);
	args.add(itfParams);
	scriptManager->callFunctionOnAllScripts("sendValuesForObject", args);
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

		if (Parameter* p = ControllableFactory::createParameterFrom((Parameter*)gci->controllable, true, true))
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
