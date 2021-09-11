/*
  ==============================================================================

    FreezeEffect.h
    Created: 11 Sep 2021 5:16:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class FreezeEffect :
    public Effect
{
public:
    FreezeEffect(const String& name = "Freeze", var params = var());
    virtual ~FreezeEffect();

    HashMap<ObjectComponent*, var> prevValues;
    
    enum FreezeMode { HOLD, MIN, MAX };
    EnumParameter* freezeMode;
    Trigger* reset;

    virtual void onContainerTriggerTriggered(Trigger* t) override;
    virtual void onContainerParameterChangedInternal(Parameter* p) override;

    var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1.f) override;
    virtual var getProcessedComponentValuesFreezeInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1.f) = 0;

};

class FreezeFloatEffect :
    public FreezeEffect
{
public:
    FreezeFloatEffect(var params = var());
    virtual ~FreezeFloatEffect();

    var getProcessedComponentValuesFreezeInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1.f) override;

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Freeze"; }
    static FreezeFloatEffect* create(var params) { return new FreezeFloatEffect(params); }
};
