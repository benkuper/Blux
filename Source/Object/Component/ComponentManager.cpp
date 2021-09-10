/*
  ==============================================================================

    ComponentManager.cpp
    Created: 26 Sep 2020 10:02:54am
    Author:  bkupe

  ==============================================================================
*/

ComponentManager::ComponentManager(Object * o) :
    BaseManager("Components"),
    factory(o)
{
    managerFactory = &factory;
    selectItemWhenCreated = false;
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::addComponentFromDefinition(StringRef type, var definition, bool canBeRemoved)
{
    ObjectComponent* c = managerFactory->create(type);
    c->userCanRemove = canBeRemoved;
    c->setupFromJSONDefinition(definition);
    addItem(c);
}

var ComponentManager::getSceneData()
{
    var data(new DynamicObject());
    for (auto& c : items) data.getDynamicObject()->setProperty(c->shortName, c->getSceneData());
    return data;
}

void ComponentManager::updateSceneData(var& sceneData)
{
}

void ComponentManager::lerpFromSceneData(var startData, var endData, float weight)
{
    for (auto& i : items) i->lerpFromSceneData(startData.getProperty(i->shortName, var()), endData.getProperty(i->shortName, var()), weight);
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




ComponentFactory::ComponentFactory(Object * o)
{
    defs.add(ComponentDefinition::createDef("", "Intensity", &IntensityComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "Color", &ColorComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "Strobe", &StrobeComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "PanTilt", &PanTiltComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "Gobo", &GoboComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "Servo", &ServoComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "Stepper", &StepperComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "Laser", &LaserComponent::create, o));
    defs.add(ComponentDefinition::createDef("", "Script", &ScriptComponent::create, o));
}
