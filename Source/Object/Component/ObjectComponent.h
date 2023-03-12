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

    ControllableContainer interfaceParams;

    //dmx
    Array<Parameter*> sourceParameters;
    Array<Parameter*> computedParameters;
    HashMap<Parameter*, Parameter*> computedParamMap;

    HashMap<Parameter*, int> localDMXChannels; //if set, will be used by fillInterfaceData for DMX interfaces to automatically fill.

    void generateInterfaceParams(Interface* i);

    void addComputedParameter(Parameter* p, Parameter* originalParameter = nullptr);
    void linkComputedParamToDMXChannel(Parameter* cp, int localChannel);

    void onContainerParameterChangedInternal(Parameter* p) override;

    virtual void update() {}

    virtual void fillComputedValueMap(HashMap<Parameter*, var>& values);
    virtual void updateComputedValues(HashMap<Parameter*, var>& values);

    virtual void setupFromJSONDefinition(var data);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    virtual void fillInterfaceData(Interface* i, var data, var params);// (HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset = false);
    virtual void fillInterfaceDataInternal(Interface* i, var data, var params);// (HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset = false);
    //virtual void fillOutValueMap(HashMap<int, float> &channelValueMap, int startChannel, bool ignoreChannelOffset = false);

    InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
};