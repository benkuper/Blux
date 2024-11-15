/*
  ==============================================================================

    BluxSequence.h
    Created: 5 Oct 2020 2:42:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class ObjectComponent;
class Effect;

class BluxSequence :
    public Sequence,
    public ChainVizTarget,
    public SequenceLayerManager::ManagerListener
{
public:
    BluxSequence();
    ~BluxSequence();

    bool manualStartAtLoad;

    bool isAffectingObject(Object* o);
    Array<ChainVizTarget *> getChainVizTargetsForObjectAndComponent(Object* o, ComponentType t);

    virtual void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier = 1.0f);

    virtual void processRawData();

    virtual void itemAdded(SequenceLayer* layer) override;
    virtual void itemsAdded(Array<SequenceLayer*> layers) override;

    void handleStartAtLoad() override;

    String getPanelName() const override;

    ChainVizComponent* createVizComponent(Object* o, ComponentType ct, ChainVizTarget::ChainVizType type) override;

    InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
    String getTypeString() const override { return "Blux Sequence"; }
};