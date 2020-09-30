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
    ObjectComponent(String name = "Component", var params = var());
    virtual ~ObjectComponent();

    //dmx
    OwnedArray<Parameter> computedParameters;
    HashMap<Parameter*, Parameter*> computedParamMap;
    Array<int> paramChannels;

    void addComputedParameter(Parameter* p, int channel, Parameter* originalParameter = nullptr);

    virtual var getOriginalComputedValues();

    virtual void setupFromJSONDefinition(var data);
    InspectableEditor* getEditor(bool isRoot) override;
};