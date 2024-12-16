/*
  ==============================================================================

	FilterManager.h
	Created: 26 Sep 2020 2:13:19pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class FilterManager :
	public BaseManager<Filter>
{
public:
	FilterManager();
	~FilterManager();

	Factory<Filter> factory;

	enum WeightOperator { MIN, MAX, MULTIPLY};
	EnumParameter* weightOperator;

	var getSceneData();
	void updateSceneData(var& sceneData);
	void lerpFromSceneData(var startData, var endData, float weight);

	bool isAffectingObject(Object* o);
	FilterResult getFilteredResultForComponent(Object* o, ObjectComponent* c);

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataManagerInternal(var data) override;
};