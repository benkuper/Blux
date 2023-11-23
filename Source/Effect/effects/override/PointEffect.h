/*
  ==============================================================================

	PointEffect.h
	Created: 23 Nov 2023 4:02:06pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class PointEffect :
	public Effect
{
public:
	PointEffect(const String& name = "Point", var params = var());
	virtual ~PointEffect();

	FloatParameter* position;
	FloatParameter* radius;
	Automation fadeCurve;
};

class PointFloatEffect :
	public PointEffect
{
public:
	PointFloatEffect(var params = var());
	virtual ~PointFloatEffect();

	FloatParameter* value;

	void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	const static String getTypeStringStatic() { return "Point"; }
	static PointFloatEffect* create(var params) { return new PointFloatEffect(params); }
};
