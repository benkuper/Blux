/*
  ==============================================================================

    BluxSequenceManager.h
    Created: 5 Oct 2020 2:42:03pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ChainVizTarget;

class BluxSequenceManager :
    public SequenceManager
{
public:
    BluxSequenceManager();
    ~BluxSequenceManager();

    Sequence* createItem();

    virtual void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier = 1.0f);

    void processRawData();

    Array<ChainVizTarget*> getChainVizTargetsForObject(Object* o);


};