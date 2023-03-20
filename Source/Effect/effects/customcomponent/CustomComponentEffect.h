/*
  ==============================================================================

	CustomComponentEffect.h
	Created: 20 Mar 2023 8:51:42am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class CustomComponentEffect :
	public Effect
{
public:
	CustomComponentEffect(var params = var());
	virtual ~CustomComponentEffect();

	IntParameter* numValues;
	Array<Parameter*> customValues;

	SpinLock valuesLock;

	void rebuildValues();

	void effectParamChanged(Controllable* p) override;

	void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

	DECLARE_TYPE("Values Override")
};
