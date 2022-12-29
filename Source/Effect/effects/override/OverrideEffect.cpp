/*
  ==============================================================================

    OverrideEffect.cpp
    Created: 1 Oct 2020 12:31:17pm
    Author:  bkupe

  ==============================================================================
*/

OverrideEffect::OverrideEffect(const String &name, var params) :
    Effect(name, params)
{
    filterManager->componentSelector.allowedComponents.removeAllInstancesOf(ComponentType::COLOR);
    filterManager->componentSelector.selectedComponents.set(ComponentType::INTENSITY, true);
    filterManager->componentSelector.selectedComponents.set(ComponentType::PAN, true);
    filterManager->componentSelector.selectedComponents.set(ComponentType::TILT, true);
    filterManager->componentSelector.selectedComponents.set(ComponentType::SERVO, true);
    filterManager->componentSelector.selectedComponents.set(ComponentType::STEPPER, true);
}

OverrideEffect::~OverrideEffect()
{
}


OverrideFloatEffect::OverrideFloatEffect(var params) :
    OverrideEffect(getTypeString(), params)
{
    value = effectParams.addFloatParameter("Value", "Overriding value", 0, 0 ,1);
    value->isCustomizableByUser = true;
}

OverrideFloatEffect::~OverrideFloatEffect()
{
}

var OverrideFloatEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{
    float val = GetLinkedValue(value);

    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray())
        {
            for (int j = 0; j < values.size(); j++)
            {
                values[i][j] = val;
            }
        }
        else
        {
            values[i] = val;
        }
    }

    return values;
}