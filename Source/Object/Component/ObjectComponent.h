/*
  ==============================================================================

    ObjectComponent.h
    Created: 26 Sep 2020 10:03:23am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class Interface;

class ObjectComponent :
    public BaseItem
{
public:

    ObjectComponent(Object* o = nullptr, String name = "Component", ComponentType componentType = CUSTOM, var params = var());
    virtual ~ObjectComponent();
    
    Object* object;

    Parameter* mainParameter;

    ComponentType componentType;
    BoolParameter* excludeFromScenes;

    ControllableContainer interfaceParamCC;
    Array<Parameter*> interfaceParams;
    HashMap<Parameter*, Parameter*> computedInterfaceMap;
    var interfaceParamsGhostData;

    //dmx
    Array<Parameter*> sourceParameters;
    Array<Parameter*> computedParameters;
    HashMap<Parameter*, Parameter*> computedParamMap;
    HashMap<Parameter*, Parameter*> paramComputedMap;

    void rebuildInterfaceParams(Interface* i);

    Parameter* addComputedParameter(Parameter* p, ControllableContainer* parent = nullptr);

    void onContainerParameterChangedInternal(Parameter* p) override;

    virtual void update() {}

    virtual void fillComputedValueMap(HashMap<Parameter*, var>& values);
    virtual void updateComputedValues(HashMap<Parameter*, var>& values);

    virtual void setupFromJSONDefinition(var data);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    virtual var getVizData();

    virtual void fillInterfaceData(Interface* i, var data, var params);// (HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset = false);
    virtual void fillInterfaceDataInternal(Interface* i, var data, var params);// (HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset = false);
    //virtual void fillOutValueMap(HashMap<int, float> &channelValueMap, int startChannel, bool ignoreChannelOffset = false);

    virtual var getMappedValueForComputedParam(Interface* i, Parameter* computedP);

    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;


    InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
};