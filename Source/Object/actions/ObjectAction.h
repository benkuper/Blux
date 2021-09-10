/*
  ==============================================================================

    ObjectAction.h
    Created: 8 Oct 2020 3:31:13pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ObjectAction :
	public Action,
	public EngineListener
{
public:
	ObjectAction(var params);
	~ObjectAction();

	enum ActionType { ENABLE_OBJECT, DISABLE_OBJECT, SET_COMPONENT_PARAMETER_VALUE };
	ActionType actionType;
	TargetParameter* target;
	WeakReference<Parameter> value;
	var dataToLoad;
	var ghostValueData; // to keep when target is lost

	void setValueParameter(Parameter* p);

	virtual void triggerInternal() override;
	virtual void setValueInternal(var value) override;

	void onContainerParameterChanged(Parameter*) override;

	static Controllable* showAndGetComponentParameter(const StringArray & typesFilter, const StringArray & excludeTypesFilter);

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	static ObjectAction* create(var params) { return new ObjectAction(params); }
};