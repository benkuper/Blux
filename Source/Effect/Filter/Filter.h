/*
  ==============================================================================

    Filter.h
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ObjectComponent;
class Object;

struct FilterResult
{
    int id = -1;
    float weight = 1;
};

class Filter :
    public BaseItem
{
public:
    Filter(const String &name = "Filter");
    virtual ~Filter();


    enum IDMode { NO_CHANGE, LOCAL, LOCAL_REVERSE, RANDOMIZED, GLOBAL };
    EnumParameter* idMode;
    BoolParameter* invert;
    BoolParameter* excludeFromScenes;

    virtual bool isAffectingObject(Object* o, int localID = -1);
    FilterResult getFilteredResultForComponent(Object* o, ObjectComponent * c, int localID = -1);
    virtual FilterResult getFilteredResultForComponentInternal(Object* o, ObjectComponent* c, int localID = -1);
    
    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);
};