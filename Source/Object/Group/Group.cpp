/*
  ==============================================================================

    Group.cpp
    Created: 29 Sep 2020 9:46:51pm
    Author:  bkupe

  ==============================================================================
*/

Group::Group(String name) :
    BaseItem(name)
{
    saveAndLoadRecursiveData = true;
 
    effectManager.reset(new EffectManager());
    addChildControllableContainer(effectManager.get());
}

Group::~Group()
{

}

bool Group::containsObject(Object* o)
{
    return false;
}

int Group::getLocalIDForObject(Object* o)
{
    return -1;
}

void Group::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    effectManager->processComponentValues(o, c, values, 1, getLocalIDForObject(o));
}

var Group::getSceneData()
{
    var data(new DynamicObject());
    data.getDynamicObject()->setProperty(effectManager->shortName,effectManager->getSceneData());
    return data;
}

void Group::updateSceneData(var& sceneData)
{
}

void Group::lerpFromSceneData(var startData, var endData, float weight)
{
    effectManager->lerpFromSceneData(startData.getProperty(effectManager->shortName, var()), endData.getProperty(effectManager->shortName, var()), weight);
}
