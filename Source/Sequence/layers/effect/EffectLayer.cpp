
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

	if (blocks.isEmpty()) return;

	if (blocks.size() == 1) {
		EffectBlock* eb = (EffectBlock*)blocks[0];
		eb->processComponent(o, c, values, fr.weight * weightMultiplier, fr.id, time);
		return;
	}

	//lerp
	HashMap<Parameter*, var>::Iterator valIt(values);

	OwnedArray<HashMap<Parameter*, var>> blockMaps;
	Array<float> blockWeights;

	for (auto& b : blocks)
	{
		HashMap<Parameter*, var>* bVals = new HashMap<Parameter*, var>();
		valIt.reset();
		while (valIt.next()) bVals->set(valIt.getKey(), valIt.getValue().clone());
		((EffectBlock*)b)->processComponent(o, c, *bVals, fr.weight * weightMultiplier, fr.id, time, true);
		blockMaps.add(bVals);
		blockWeights.add(((EffectBlock*)b)->getFadeMultiplier(time));
	}


	//lerp between all now

	valIt.reset();
	while (valIt.next())
	{
		Parameter* cp = valIt.getKey();
		Array<var> blockVals;
		bool sameVal = true;
		var firstVal = blockMaps.getFirst()->getReference(cp);
		for (auto& b : blockMaps)
		{
			var v = b->getReference(cp);
			jassert(v.size() == firstVal.size());
			if (v != firstVal) sameVal = false;
			blockVals.add(v);
		}

		if (sameVal)
		{
			DBG("Same val, not lerping");
			values.set(cp, firstVal);
			continue;
		}

		DBG("Different values, lerping");

		var averageVal = getAverageValue(blockVals, blockWeights);
		values.set(cp, averageVal);
	}

	//for (auto& b : blocks) ((EffectBlock*)b)->processComponent(o, c, values, fr.weight * weightMultiplier, fr.id, time);
}

var EffectLayer::getAverageValue(Array<var> values, Array<float> weights)
{
	if (values.isEmpty()) return 0;

	bool valuesAreArrays = values[0].isArray();

	if (!valuesAreArrays)
	{
		float totalValue = 0;
		float totalWeight = 0;
		for (int i = 0; i < values.size(); i++)
		{
			totalValue += (float)values[i] * weights[i];
			totalWeight += weights[i];
		}

		if (totalWeight == 0) return 0;
		return totalValue / totalWeight;
	}

	//values are arrays
	//recurse

	var result;
	for (int vi = 0; vi < values[0].size(); vi++)
	{
		Array<var> subValues;
		for (int i = 0; i < values.size(); i++) subValues.add(values[i][vi]);
		var v = getAverageValue(subValues, weights);
		result.append(v);
	}

	return result;
}


SequenceLayerTimeline* EffectLayer::getTimelineUI()
{
	return new EffectLayerTimeline(this);
}
