/*
  ==============================================================================

    CustomComponent.h
    Created: 15 Mar 2023 7:33:19pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CustomComponent :
	public ObjectComponent
{
public:
	CustomComponent(Object* o, var params);
	~CustomComponent();

	IntParameter* numValues;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void rebuildValues();

	var getJSONData() override;
	void loadJSONDataItemInternal(var data) override;

	String getTypeString() const override { return "Custom"; }
	static CustomComponent* create(Object* o, var params) { return new CustomComponent(o, params); }
};
