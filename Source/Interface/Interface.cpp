/*
  ==============================================================================

	Interface.cpp
	Created: 26 Sep 2020 1:50:32pm
	Author:  bkupe

  ==============================================================================
*/

#include "Interface.h"
#include "Object/ObjectIncludes.h"
#include "ui/InterfaceUI.h"

Interface::Interface(String name, bool canHaveScripts) :
	BaseItem(name, true, canHaveScripts)
{
	saveAndLoadRecursiveData = true;

	logIncomingData = addBoolParameter("Log Incoming", "Log incoming data", false);
	logOutgoingData = addBoolParameter("Log Outgoing", "Log outgoing data", false);

	inActivityTrigger.reset(new Trigger("IN Activity", "Incoming Activity Signal"));
	outActivityTrigger.reset(new Trigger("OUT Activity", "Outgoing Activity Signal"));
}

Interface::~Interface()
{
}

void Interface::sendValuesForObject(Object* o)
{
	if (!enabled->boolValue()) return;

	sendValuesForObjectInternal(o);
}

InterfaceUI* Interface::createUI()
{
	return new InterfaceUI(this);
}
