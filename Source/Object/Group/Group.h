/*
  ==============================================================================

    Group.h
    Created: 29 Sep 2020 9:46:51pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class EffectManager;

class Group :
    public BaseItem
{
public:
    Group(String name = "Group");
    virtual ~Group();

    std::unique_ptr<EffectManager> effectManager;
    Array<int> randomIDs;

    virtual void generateRandomIDs();

    virtual bool containsObject(Object* o);
    virtual int getLocalIDForObject(Object* o);
    virtual int getRandomIDForObject(Object* o);

    virtual int getNumObjects() { return 0; }

    virtual Array<Object*> getObjects() { return Array<Object*>(); }

    void processComponentValues(Object* o, ObjectComponent* c, var& values);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);
    

    class RandomComparator
    {
    public:
        Random r;
        
        virtual int compareElements(int a, int b)
        {
            return r.nextBool() ? 1 : -1;
        }
    };
};