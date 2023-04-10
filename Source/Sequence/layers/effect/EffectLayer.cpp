
/*
  ==============================================================================

	EffectLayer.cpp
	Created: 5 Oct 2020 4:03:18pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"
#include "Effect/EffectIncludes.h"

EffectLayer::EffectLayer(Sequence* s, var params) :
	SequenceLayer(s, "Effect"),
	blockManager(this)
{
	saveAndLoadRecursiveData = true;

	timeOffsetByID = addFloatParameter("Time Offset by ID", "Offset by id to apply to the entire layer", 0);
	timeOffsetByID->defaultUI = FloatParameter::TIME;

	addChildControllableContainer(&blockManager);

	filterManager.reset(new FilterManager());
	addChildControllableContainer(filterManager.get());
}

EffectLayer::~EffectLayer()
{
}

Array<ChainVizTarget*> EffectLayer::getChainVizTargetsForObjectAndComponent(Object* o, ComponentType c)
{
	Array<ChainVizTarget*> result;

	if (!filterManager->isAffectingObject(o)) return result;

	FilterResult fr = filterManager->getFilteredResultForComponent(o, nullptr);
	int id = fr.id == -1 ? o->globalID->intValue() : fr.id;

	float time = sequence->currentTime->floatValue() - timeOffsetByID->floatValue() * id;
	Array<LayerBlock*> blocks = blockManager.getBlocksAtTime(time);

	for (auto& i : blocks)
	{
		EffectBlock* eb = (EffectBlock*)i;
		if (eb->effect->isAffectingObjectAndComponent(o, c)) result.add(eb->effect.get());
	}

	return result;
}

void EffectLayer::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier)
{
	FilterResult fr = filterManager->getFilteredResultForComponent(o, c);
	if (fr.id == -1) return;

	float time = sequence->currentTime->floatValue() - timeOffsetByID->floatValue() * fr.id;
	Array<LayerBlock*> blocks = blockManager.getBlocksAtTime(time, false);
	for (auto& b : blocks) ((EffectBlock*)b)->processComponent(o, c, values, fr.weight * weightMultiplier, fr.id, time);
}


SequenceLayerTimeline* EffectLayer::getTimelineUI()
{
	return new EffectLayerTimeline(this);
}
