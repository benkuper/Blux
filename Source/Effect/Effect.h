/*
  ==============================================================================

	Effect.h
	Created: 30 Sep 2020 12:38:01pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

//helper, considering variables are name o for  Object and id for object's id
#define GetLinkedValue(p) effectParams.getLinkedValue(p, o, id, time)
#define GetLinkedValueT(p, time) effectParams.getLinkedValue(p, o, id, time)


class Effect :
	public BaseItem,
	public ChainVizTarget,
	public ParamLinkContainer::ParamLinkContainerListener
{
public:
	Effect(const String& name = "Effect", var params = var());
	~Effect();

	TagSelector* tagSelector;
	ParamLinkContainer effectParams;

	enum BlendMode { OVERRIDE, ADD, MAX, MIN, MULTIPLY };
	EnumParameter* mode;

	FloatParameter* weight;

	enum IDMode { NO_CHANGE, LOCAL, LOCAL_REVERSE, RANDOMIZED };
	EnumParameter* idMode;
	Group* parentGroup;

	enum SceneSaveMode { FULL, WEIGHT_ONLY, NONE };
	EnumParameter* sceneSaveMode;

	std::unique_ptr<FilterManager> filterManager;

	bool computePreviousValues;
	OwnedArray<HashMap<Parameter*, var>>  prevValues;
	HashMap<ObjectComponent*, HashMap<Parameter*, var>*> prevValuesMap;

	bool forceDisabled;

	Array<ComponentType> typeFilters;

	//viz
	WeakReference<Parameter> vizParameter;
	WeakReference<Parameter> vizComputedParamRef
		;
	void setParentGroup(Group* g);

	void setForceDisabled(bool value);
	virtual void updateEnabled() {}

	bool isFullyEnabled();

	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	virtual void paramControlModeChanged(ParamLinkContainer* pc, ParameterLink* pl) override;
	virtual void effectParamChanged(Controllable* c) {}

	void clearPrevValues();


	virtual bool isAffectingObject(Object* o, int localID = -1);
	virtual bool isAffectingObjectAndComponent(Object* o, ComponentType t, int localID = -1);
	void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier = 1.0f, int id = -1, float time = -1);
	virtual void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1);

	virtual var blendValue(var start, var end, float weight);
	virtual float blendFloatValue(float start, float end, float weight);

	var getSceneData();
	void updateSceneData(var& sceneData);
	void lerpFromSceneData(var startData, var endData, float weight);

	ChainVizComponent* createVizComponent(Object* o, ComponentType ct, ChainVizTarget::ChainVizType type) override;

	void registerVizFeedback(Parameter* vizParam, Parameter* computedRef);
	void clearVizFeedback();

	virtual InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
	String getTypeString() const override { return "Effect"; }

	class  EffectListener
	{
	public:
		virtual ~EffectListener() {}
		virtual void effectParamControlModeChanged(Parameter* p) {}
		//virtual void blockBakingProgress(float progress) {}
		//virtual void colorsUpdated() {}
	};

	ListenerList<EffectListener> effectListeners;
	void addEffectListener(EffectListener* newListener) { effectListeners.add(newListener); }
	void removeEffectListener(EffectListener* listener) { effectListeners.remove(listener); }
};