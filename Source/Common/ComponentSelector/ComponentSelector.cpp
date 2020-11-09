/*
  ==============================================================================

    ComponentSelector.cpp
    Created: 3 Oct 2020 2:08:48pm
    Author:  bkupe

  ==============================================================================
*/

#include "ComponentSelector.h"

ComponentSelector::ComponentSelector()
{
    for (int i = 0; i < ObjectComponent::TYPES_MAX; i++) allowedComponents.add((ObjectComponent::ComponentType)i);
}

ComponentSelector::~ComponentSelector()
{
}

var ComponentSelector::getJSONData()
{
    var data;
    for (HashMap<ObjectComponent::ComponentType, bool>::Iterator it(selectedComponents); it.next();) if(it.getValue()) data.append(ObjectComponent::typeNames[it.getKey()]);
    return data;
}

void ComponentSelector::loadJSONData(var data)
{
    if (!data.isArray()) return;
    for (int i = 0; i < allowedComponents.size(); i++)
    {
        selectedComponents.set(allowedComponents[i], data.getArray()->contains(ObjectComponent::typeNames[(int)allowedComponents[i]]));
    }
}
