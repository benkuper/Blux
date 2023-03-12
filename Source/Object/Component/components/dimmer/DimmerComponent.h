/*
  ==============================================================================

	DimmerComponent.h
	Created: 26 Sep 2020 2:20:14pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class DimmerComponent :
	public ObjectComponent
{
public:
	DimmerComponent(Object* o, var params);
	~DimmerComponent();

	FloatParameter* value;
	Automation curve;

	virtual void updateComputedValues(HashMap<Parameter*, var>& values) override;

	void setupFromJSONDefinition(var data) override;

	String getTypeString() const override { return "Dimmer"; }
	static DimmerComponent* create(Object* o, var params) { return new DimmerComponent(o, params); }
};
