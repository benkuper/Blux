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

    enum FreezeMode { HOLD, MIN, MAX };
    EnumParameter* freezeMode;
    Trigger* reset;

    virtual void onContainerTriggerTriggered(Trigger* t) override;
    virtual void onContainerParameterChangedInternal(Parameter* p) override;

    void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;
    virtual void processComponentFreezeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) = 0;

};

class FreezeFloatEffect :
    public FreezeEffect
{
public:
    FreezeFloatEffect(var params = var());
    virtual ~FreezeFloatEffect();

    virtual void processComponentFreezeInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Freeze"; }
    static FreezeFloatEffect* create(var params) { return new FreezeFloatEffect(params); }
};
