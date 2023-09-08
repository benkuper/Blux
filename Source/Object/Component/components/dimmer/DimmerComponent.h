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
	BoolParameter* useFineValue;

	Automation curve;

	virtual void updateComputedValues(HashMap<Parameter*, var>& values) override;
	virtual void fillInterfaceData(Interface* i, var data, var params) override;// (HashMap<int, float>& channelValueMap, int startChannel, bool 

	String getTypeString() const override { return "Dimmer"; }
	static DimmerComponent* create(Object* o, var params) { return new DimmerComponent(o, params); }
};
