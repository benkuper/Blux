/*
  ==============================================================================

    ComponentFilter.h
    Created: 30 Sep 2020 2:56:39pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Filter.h"

class ComponentFilter :
    public Filter
{
public:
    ComponentFilter();
    ~ComponentFilter();

    EnumParameter* mode;
    ControllableContainer components;

    EnumParameter * createComponentEnum(ControllableContainer* cc);

    var getJSONData() override;
    void loadJSONDataInternal(var data) override;

    String getTypeString() const override { return "Component"; }
    static ComponentFilter* create(var params) { return new ComponentFilter(); }
};