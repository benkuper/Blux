/*
  ==============================================================================

    ObjectComponent.h
    Created: 26 Sep 2020 10:03:23am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Object;

class ObjectComponent :
    public BaseItem
{
public:
    enum ComponentType { INTENSITY, COLOR, GOBO, LASER, PANTILT, SERVO, STEPPER, STROBE, SCRIPT, CUSTOM, TYPES_MAX };
    static const String typeNames[TYPES_MAX]; 

    ObjectComponent(Object* o = nullptr, String name = "Component", ComponentType componentType = CUSTOM, var params = var());
    virtual ~ObjectComponent();
    
    Object* object;
    int channelOffset;

    ComponentType componentType;
    BoolParameter* excludeFromScenes;

    //dmx
    Array<Parameter*> sourceParameters;
    Array<Parameter*> computedParameters;
    HashMap<Parameter*, Parameter*> computedParamMap;
    Array<int> paramChannels;

    //compute
    bool isDirty;

    void addComputedParameter(Parameter* p, int channel, Parameter* originalParameter = nullptr);

    void onContainerParameterChangedInternal(Parameter* p) override;

    virtual void update() {}

    virtual var getOriginalComputedValues();

    virtual void setupFromJSONDefinition(var data);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    virtual void fillOutValueMap(HashMap<int, float> &channelValueMap, int startChannel);

    InspectableEditor* getEditor(bool isRoot) override;
};