/*
  ==============================================================================

    EffectLayer.cpp
    Created: 5 Oct 2020 4:03:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectLayer.h"
#include "ui/EffectLayerTimeline.h"
#include "Object/Object.h"

EffectLayer::EffectLayer(Sequence* s, var params) :
    SequenceLayer(s, "Effect"),
    blockManager(this)
{
    saveAndLoadRecursiveData = true;
    
    timeOffsetByID = addFloatParameter("Time Offset by ID", "Offset by id to apply to the entire layer", 0);
    timeOffsetByID->defaultUI = FloatParameter::TIME;

    addChildControllableContainer(&blockManager);

    filterManager.componentSelector.selectedComponents.set(ObjectComponent::INTENSITY, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::PANTILT, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::SERVO, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::STEPPER, true);
    addChildControllableContainer(&filterManager);
}

EffectLayer::~EffectLayer()
{
}

Array<ChainVizTarget *> EffectLayer::getChainVizTargetsForObject(Object* o)
{
    Array<ChainVizTarget *> result;
    if (!filterManager.isAffectingObject(o)) return result;

    Array<LayerBlock*> blocks = blockManager.getBlocksAtTime(sequence->currentTime->floatValue());
    for (auto& i : blocks)
    {
        EffectBlock* eb = (EffectBlock*)i;
        if (eb->effect->filterManager.isAffectingObject(o)) result.add(eb->effect.get());
    }

    return result;
}

void EffectLayer::processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier)
{
    FilterResult fr = filterManager.getFilteredResultForComponent(o, c);
    if (fr.id == -1) return;

    float time = sequence->currentTime->floatValue() - timeOffsetByID->floatValue() * fr.id;

    Array<LayerBlock *> blocks = blockManager.getBlocksAtTime(time, false);
    for(auto & b : blocks) ((EffectBlock *)b)->processComponentValues(o, c, values, fr.weight * weightMultiplier, fr.id, time);
}

SequenceLayerTimeline* EffectLayer::getTimelineUI()
{
    return new EffectLayerTimeline(this);
}
