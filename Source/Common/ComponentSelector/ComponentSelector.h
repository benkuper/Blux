/*
  ==============================================================================

    ComponentSelector.h
    Created: 3 Oct 2020 2:08:48pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Object/Component/ObjectComponent.h"

class ComponentSelector
{
public:
    ComponentSelector();
    ~ComponentSelector();

    HashMap<ObjectComponent::ComponentType, bool> selectedComponents;

    var getJSONData();
    void loadJSONData(var data);
};