/*
  ==============================================================================

    ComponentManager.cpp
    Created: 26 Sep 2020 10:02:54am
    Author:  bkupe

  ==============================================================================
*/

#include "ComponentManager.h"

#include "components/intensity/IntensityComponent.h"
#include "components/strobe/StrobeComponent.h"
#include "components/color/ColorComponent.h"
#include "components/pantilt/PanTiltComponent.h"
#include "components/strobe/StrobeComponent.h"
#include "components/gobo/GoboComponent.h"
#include "components/servo/ServoComponent.h"
#include "components/stepper/StepperComponent.h"
#include "components/laser/LaserComponent.h"
#include "components/script/ScriptComponent.h"

ComponentManager::ComponentManager() :
    BaseManager("Components")
{
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Intensity", &IntensityComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Color", &ColorComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Strobe", &StrobeComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "PanTilt", &PanTiltComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Gobo", &GoboComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Servo", &ServoComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Stepper", &StepperComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Laser", &LaserComponent::create));
    factory.defs.add(Factory<ObjectComponent>::Definition::createDef("", "Script", &ScriptComponent::create));
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::addComponentFromDefinition(StringRef type, var definition)
{
    ObjectComponent* c = factory.create(type);
    c->setupFromJSONDefinition(definition);
    addItem(c);
}

var ComponentManager::getJSONData()
{
    var data = ControllableContainer::getJSONData();
    var cData;
    for (auto& c : items) cData.append(c->getJSONData());
    data.getDynamicObject()->setProperty("components",cData);

    return data;
}

void ComponentManager::loadJSONDataInternal(var data)
{
    ControllableContainer::loadJSONDataInternal(data);

    //Doing an index based loading, this may be improved but should be enough as long as objects don't change their components structure 
    var cData = data.getProperty("components", var());
    for (int i = 0; i < cData.size() && i < items.size(); i++) items[i]->loadJSONData(cData[i]);
}
