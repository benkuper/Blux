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

    enum OverrideMode { ADD, MAX, MIN };
    EnumParameter* mode;

};

class OverrideFloatEffect :
    public OverrideEffect
{
public:
    OverrideFloatEffect(var params = var());
    virtual ~OverrideFloatEffect();

    FloatParameter* value;
    
    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values) override;

    String getTypeString() const override { return "Override (Number)"; }
    static OverrideFloatEffect* create(var params) { return new OverrideFloatEffect(params); }
};

class OverrideColorEffect :
    public OverrideEffect
{
public:
    OverrideColorEffect(var params = var());
    virtual ~OverrideColorEffect();

    ColorParameter* value;

    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values) override;

    String getTypeString() const override { return "Override (Color)"; }
    static OverrideColorEffect* create(var params) { return new OverrideColorEffect(params); }
};