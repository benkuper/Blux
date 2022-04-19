/*
  ==============================================================================

    Effect.h
    Created: 30 Sep 2020 12:38:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

//helper, considering variables are name o for  Object and id for object's id
#define GetLinkedValue(p) effectParams.getLinkedValue(p, o, id)

class Effect :
    public BaseItem,
    public ChainVizTarget
{
public:
    Effect(const String& name = "Effect", var params = var());
    ~Effect();

    ParamLinkContainer effectParams;

    enum BlendMode { OVERRIDE, ADD, MAX, MIN, MULTIPLY };
    EnumParameter* mode;

    FloatParameter* weight;

    enum IDMode { NO_CHANGE, LOCAL, LOCAL_REVERSE, RANDOMIZED };
    EnumParameter* idMode;
    Group* parentGroup;

    enum SceneSaveMode { FULL, WEIGHT_ONLY, NONE };
    EnumParameter * sceneSaveMode;

    std::unique_ptr<FilterManager> filterManager;

    bool forceDisabled;

    void setParentGroup(Group* g);

    void setForceDisabled(bool value);
    virtual void updateEnabled() {}

    bool isFullyEnabled();
    
    virtual void onContainerParameterChangedInternal(Parameter* p) override;

    bool isAffectingObject(Object * o);
    void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f, int id = -1, float time = -1);
    virtual var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1);

    virtual var blendValue(var start, var end, float weight);
    virtual float blendFloatValue(float start, float end, float weight);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);


    ChainVizComponent* createVizComponent(Object* o, ChainVizTarget::ChainVizType type) override;

    virtual InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
    String getTypeString() const override { return "Effect"; }
};