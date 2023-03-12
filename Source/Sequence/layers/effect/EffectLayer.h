/*
  ==============================================================================

	EffectLayer.h
	Created: 5 Oct 2020 4:03:18pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class ObjectComponent;
class FilterManager;

class EffectLayer :
	public SequenceLayer
{
public:
	EffectLayer(Sequence* s, var params = var());
	~EffectLayer();

	EffectBlockManager blockManager;
	std::unique_ptr<FilterManager> filterManager;

	FloatParameter* timeOffsetByID;

	Array<EffectBlock*, CriticalSection> activeBlocks;

	Array<ChainVizTarget*> getChainVizTargetsForObject(Object* o);
	virtual void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier = 1.0f);

	SequenceLayerTimeline* getTimelineUI() override;

	String getTypeString() const override { return "Effect"; }
	static EffectLayer* create(Sequence* s, var params) { return new EffectLayer(s, params); }
};