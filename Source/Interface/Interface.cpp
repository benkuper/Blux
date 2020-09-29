/*
  ==============================================================================

    Interface.cpp
    Created: 26 Sep 2020 1:50:32pm
    Author:  bkupe

  ==============================================================================
*/

#include "Interface.h"
#include "Object/Object.h"

Interface::Interface(String name) :
    BaseItem(name)
{
    logIncomingData = addBoolParameter("Log Incoming", "Log incoming data", false);
    logIncomingData->hideInEditor = true;
    logOutgoingData = addBoolParameter("Log Outgoing", "Log outgoing data", false);
    logOutgoingData->hideInEditor = true;
}

Interface::~Interface()
{
}