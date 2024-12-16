/*
  ==============================================================================

    Group.cpp
    Created: 29 Sep 2020 9:46:51pm
    Author:  bkupe

  ==============================================================================
*/


#include <algorithm>

Group::Group(String name) :
    BaseItem(name)
{
    saveAndLoadRecursiveData = true;
    setHasCustomColor(true);

    effectManager.reset(new EffectManager(this));
    addChildControllableContainer(effectManager.get());
}

Group::~Group()
{

}

void Group::generateRandomIDs()
{
    randomIDs.resize(getNumObjects());
    for (int i = 0; i < randomIDs.size(); i++) randomIDs.set(i, i);

    shuffle(randomIDs.begin(), randomIDs.end(), std::default_random_engine(Time::currentTimeMillis()));
}

bool Group::containsObject(Object* o)
{
    return false;
}

int Group::getLocalIDForObject(Object* o)
{
    return -1;
}

int Group::getRandomIDForObject(Object* o)
{
    return -1;
}

void Group::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values)
{
    effectManager->processComponent(o, c, values, 1, getLocalIDForObject(o));
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
