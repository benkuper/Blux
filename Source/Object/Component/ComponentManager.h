/*
  ==============================================================================

    ComponentManager.h
    Created: 26 Sep 2020 10:02:54am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ObjectComponent.h"

class ComponentManager :
    public BaseManager<ObjectComponent>
{
public:
    ComponentManager();
    ~ComponentManager();

    Factory<ObjectComponent> factory;

    void addComponentFromDefinition(StringRef type, var definition);

    var getJSONData() override;
    void loadJSONDataInternal(var data) override;
};