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

var OverrideFloatEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values)
{
    OverrideMode m = mode->getValueDataAsEnum<OverrideMode>();

    switch (m)
    {
    case ADD:
        values[0] = (float)values[0] + value->floatValue();
        break;

    case MIN:
        values[0] = jmin<float>(values[0], value->floatValue());
        break;

    case MAX:
        values[0] = jmax<float>(values[0], value->floatValue());
        break;
    }

    return values;
}
