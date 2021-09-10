/*
  ==============================================================================

    EffectLayer.cpp
    Created: 5 Oct 2020 4:03:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "EffectLayer.h"
#include "ui/EffectLayerTimeline.h"
#include "Object/ObjectIncludes.h"

EffectLayer::EffectLayer(Sequence* s, var params) :
    SequenceLayer(s, "Effect"),
    blockManager(this)
{
    saveAndLoadRecursiveData = true;
    
    timeOffsetByID = addFloatParameter("Time Offset by ID", "Offset by id to apply to the entire layer", 0);
    timeOffsetByID->defaultUI = FloatParameter::TIME;

    addChildControllableContainer(&blockManager);

    filterManager.componentSelector.selectedComponents.set(ComponentType::INTENSITY, true);
    filterManager.componentSelector.selectedComponents.set(ComponentType::COLOR, true);
    filterManager.componentSelector.selectedComponents.set(ComponentType::PANTILT, true);
    filterManager.componentSelector.selectedComponents.set(ComponentType::SERVO, true);
    filterManager.componentSelector.selectedComponents.set(ComponentType::STEPPER, true);
    addChildControllableContainer(&filterManager);
}

EffectLayer::~EffectLayer()
{
}

Array<ChainVizTarget *> EffectLayer::getChainVizTargetsForObject(Object* o)
{
    Array<ChainVizTarget *> result;
    
    if (!filterManager.isAffectingObject(o)) return result;

    FilterResult fr = filterManager.getFilteredResultForComponent(o, nullptr);
    int id = fr.id == -1 ? o->globalID->intValue() : fr.id;

    float time = sequence->currentTime->floatValue() - timeOffsetByID->floatValue() * id;
    Array<LayerBlock*> blocks = blockManager.getBlocksAtTime(time);

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
