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
    Array<Parameter *> computedParameters;
    HashMap<Parameter*, Parameter*> computedParamMap;
    Array<int> paramChannels;

    //compute
    bool isDirty;

    void addComputedParameter(Parameter* p, int channel, Parameter* originalParameter = nullptr);

    void onContainerParameterChangedInternal(Parameter* p) override;

    virtual var getOriginalComputedValues();

    virtual void setupFromJSONDefinition(var data);
    InspectableEditor* getEditor(bool isRoot) override;
};