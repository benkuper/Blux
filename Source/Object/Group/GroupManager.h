/*
  ==============================================================================

    GroupManager.h
    Created: 29 Sep 2020 9:46:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class ObjectComponent;

class GroupManager :
    public BaseManager<Group>
{
public:
    juce_DeclareSingleton(GroupManager, true);

    GroupManager();
    ~GroupManager();

    Factory<Group> factory;

    Array<ChainVizTarget *> getChainVizTargetsForObject(Object* o);
    virtual void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);
};