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
    mode = addEnumParameter("Mode", "Override mode, defines how the values are handled.");
    mode->addOption("Max", MAX)->addOption("Min", MIN)->addOption("Add", ADD);

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
    value = addFloatParameter("Value", "Overriding value", 0);
    value->isCustomizableByUser = true;
}

OverrideFloatEffect::~OverrideFloatEffect()
{
}

var OverrideFloatEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values)
{
    OverrideMode m = mode->getValueDataAsEnum<OverrideMode>();

    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray()) continue;

        switch (m)
        {
        case ADD:
            values[i] = (float)values[i] + value->floatValue();
            break;

        case MIN:
            values[i] = jmin<float>(values[i], value->floatValue());
            break;

        case MAX:
            values[i] = jmax<float>(values[i], value->floatValue());
            break;
        }
    }
  

    return values;
}

OverrideColorEffect::OverrideColorEffect(var params) :
    OverrideEffect(getTypeString(), params)
{
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::COLOR, true);

    value = addColorParameter("Value", "Value to override", Colours::red);
}

OverrideColorEffect::~OverrideColorEffect()
{
}

var OverrideColorEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values)
{
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].size() != value->value.size()) continue;
        values[i] = value->value.clone();
       
    }

    return values;
}
