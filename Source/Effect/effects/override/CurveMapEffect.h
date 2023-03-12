/*
  ==============================================================================

    CurveMapEffect.h
    Created: 1 Oct 2020 12:31:17pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CurveMapEffect :
    public Effect
{
public:
    CurveMapEffect(var params = var());
    virtual ~CurveMapEffect();

    Point2DParameter* inputRange;
    Point2DParameter* outputRange;
    Automation automation;

    void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Curve Map"; }
    static CurveMapEffect* create(var params) { return new CurveMapEffect(params); }
};
