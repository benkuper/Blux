/*
  ==============================================================================

    InterfaceManager.cpp
    Created: 26 Sep 2020 1:50:28pm
    Author:  bkupe

  ==============================================================================
*/

#include "InterfaceManager.h"

juce_ImplementSingleton(InterfaceManager)

InterfaceManager::InterfaceManager() :
    Manager("Interfaces")
{
    managerFactory = &factory;

    factory.defs.add(Factory<Interface>::Definition::createDef("", "DMX", &DMXInterface::create));
    factory.defs.add(Factory<Interface>::Definition::createDef("", "OSC", &CustomOSCInterface::create));
    factory.defs.add(Factory<Interface>::Definition::createDef("", "Serial", &SerialInterface::create));
    factory.defs.add(Factory<Interface>::Definition::createDef("", "Bento", &BentoInterface::create));
    factory.defs.add(Factory<Interface>::Definition::createDef("", "MIDI", &MIDIInterface::create));
}

InterfaceManager::~InterfaceManager()
{
}
