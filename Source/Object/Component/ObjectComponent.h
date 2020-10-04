/*
  ==============================================================================

    ObjectComponent.h
    Created: 26 Sep 2020 10:03:23am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ObjectComponent :
    public BaseItem
{
public:
    enum ComponentType { INTENSITY, COLOR, GOBO, LASER, PANTILT, SERVO, STEPPER, STROBE, SCRIPT, CUSTOM, TYPES_MAX };
    static const String typeNames[TYPES_MAX]; 

    ObjectComponent(String name = "Component", ComponentType componentType = CUSTOM, var params = var());
    virtual ~ObjectComponent();
    
    ComponentType componentType;

    //dmx
    Array<Parameter *> computedParameters;
    HashMap<Parameter*, Parameter*> computedParamMap;
    Array<int> paramChannels;

    //compute
    bool isDirty;

    void addComputedParameter(Parameter* p, int channel, Parameter* originalParameter = nullptr);

    void onContainerParameterChangedInternal(Parameter* p) override;

    virtual var getOriginalComputedValues();

    virtual void setupFromJSONDefinition(var data);

    void saveSceneData(var &sceneData);

    InspectableEditor* getEditor(bool isRoot) override;
};