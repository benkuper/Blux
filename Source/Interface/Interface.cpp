/*
  ==============================================================================

    Interface.cpp
    Created: 26 Sep 2020 1:50:32pm
    Author:  bkupe

  ==============================================================================
*/

#include "Interface.h"
#include "Object/Object.h"
#include "ui/InterfaceUI.h"

Interface::Interface(String name) :
    BaseItem(name)
{
    saveAndLoadRecursiveData = true;

    logIncomingData = addBoolParameter("Log Incoming", "Log incoming data", false);
    logOutgoingData = addBoolParameter("Log Outgoing", "Log outgoing data", false);
}

Interface::~Interface()
{
}

InterfaceUI* Interface::createUI()
{
    return new InterfaceUI(this);
}
