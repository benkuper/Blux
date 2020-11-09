/*
  ==============================================================================

    OverrideEffect.cpp
    Created: 1 Oct 2020 12:31:17pm
    Author:  bkupe

  ==============================================================================
*/

#include "OverrideEffect.h"


OverrideEffect::OverrideEffect(const String &name, var params) :
    Effect(name, params)
{
    filterManager.componentSelector.allowedComponents.removeAllInstancesOf(ObjectComponent::COLOR);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::INTENSITY, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::PANTILT, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::SERVO, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::STEPPER, true);
}

OverrideEffect::~OverrideEffect()
{
}


OverrideFloatEffect::OverrideFloatEffect(var params) :
    OverrideEffect(getTypeString(), params)
{
    value = addFloatParameter("Value", "Overriding value", 0, 0 ,1);
    value->isCustomizableByUser = true;
}

OverrideFloatEffect::~OverrideFloatEffect()
{
}

var OverrideFloatEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray())
        {
            for (int j = 0; j < values.size(); j++)
            {
                values[i][j] = value->floatValue();
            }
        }
        else
        {
            values[i] = value->floatValue();
        }
    }

    return values;
}