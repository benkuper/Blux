/*
  ==============================================================================

    ObjectComponent.cpp
    Created: 26 Sep 2020 10:03:23am
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectComponent.h"
#include "ui/ObjectComponentEditor.h"

ObjectComponent::ObjectComponent(String name, var params) :
    BaseItem(name),
    isDirty(true)
{
}

ObjectComponent::~ObjectComponent()
{
}

void ObjectComponent::addComputedParameter(Parameter* p, int channel, Parameter * originalParameter)
{
    p->setControllableFeedbackOnly(true);
    computedParameters.add(p);
    computedParamMap.set(p, originalParameter);
    addParameter(p);
    p->hideInEditor = true;
    paramChannels.add(channel);
}

void ObjectComponent::onContainerParameterChangedInternal(Parameter* p)
{
    BaseItem::onContainerParameterChangedInternal(p);
    
    if (p == enabled) isDirty = true;
    if (!enabled->boolValue()) return;
    
    isDirty = true;
}

var ObjectComponent::getOriginalComputedValues()
{
    var values;
    for (auto& c : computedParameters)
    {
        if (computedParamMap.contains(c)) values.append(computedParamMap[c]->getValue().clone());
        else values.append(var());
    }
    return values;
}

void ObjectComponent::setupFromJSONDefinition(var data)
{
    
}

InspectableEditor* ObjectComponent::getEditor(bool isRoot)
{
    return new ObjectComponentEditor(this, isRoot);
}
