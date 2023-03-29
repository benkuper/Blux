/*
  ==============================================================================

	BluxSequenceManager.cpp
	Created: 5 Oct 2020 2:42:03pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

BluxSequenceManager::BluxSequenceManager()
{
	itemDataType = "Sequence";
	helpID = "Sequence";
}

BluxSequenceManager::~BluxSequenceManager()
{
}


Sequence* BluxSequenceManager::createItem()
{
	return new BluxSequence();
}



void BluxSequenceManager::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier)
{
	for (auto& i : items)
	{
		if (!i->enabled->boolValue()) continue;
		((BluxSequence*)i)->processComponent(o, c, values, weightMultiplier);
	}
}

void BluxSequenceManager::processRawData()
{
	for (auto& i : items)
	{
		if (!i->enabled->boolValue()) continue;
		((BluxSequence*)i)->processRawData();
	}
}

Array<ChainVizTarget*> BluxSequenceManager::getChainVizTargetsForObjectAndComponent(Object* o, ComponentType c)
{
	Array<ChainVizTarget*> result;
	for (auto& i : items)
	{
		BluxSequence* bs = (BluxSequence*)i;
		if (bs->isAffectingObject(o)) result.add(bs);
	}
	return result;
}
