/*
  ==============================================================================

    OverrideEffect.h
    Created: 1 Oct 2020 12:31:17pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Effect.h"

class OverrideEffect :
    public Effect
{
public:
    OverrideEffect(const String &name = "Override", var params = var());
    virtual ~OverrideEffect();


};

class OverrideFloatEffect :
    public OverrideEffect
{
public:
    OverrideFloatEffect(var params = var());
    virtual ~OverrideFloatEffect();

    FloatParameter* value;
    
    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1.f) override;

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Override (Number)"; }
    static OverrideFloatEffect* create(var params) { return new OverrideFloatEffect(params); }
};

class OverrideColorEffect :
    public OverrideEffect
{
public:
    OverrideColorEffect(var params = var());
    virtual ~OverrideColorEffect();

    ColorParameter* value;

    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1.f) override;

    String getTypeString() const override {return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Override (Color)"; }

    static OverrideColorEffect* create(var params) { return new OverrideColorEffect(params); }
};