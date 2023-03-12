/*
  ==============================================================================

    OverrideEffect.cpp
    Created: 1 Oct 2020 12:31:17pm
    Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

OverrideEffect::OverrideEffect(const String &name, var params) :
    Effect(name, params)
{
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

void OverrideFloatEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
    if (c->mainParameter == nullptr) return;
    jassert(!c->mainParameter->isComplex());

    float val = GetLinkedValue(value);
    targetValues.set(c->mainParameter, val);
}