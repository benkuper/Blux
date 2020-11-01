/*
  ==============================================================================

    BluxSequenceChainVizUI.h
    Created: 1 Nov 2020 11:10:13am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ChainViz/ChainViz.h"
class Object;
class BluxSequence;

class BluxSequenceChainVizUI :
    public BaseItemChainVizComponent,
    public Sequence::SequenceListener
{
public:
    BluxSequenceChainVizUI(BluxSequence * s, Object* o, ChainVizTarget::ChainVizType type);
    ~BluxSequenceChainVizUI();

    BluxSequence* sequence;
    WeakReference<Inspectable> inspectable;

    Array<ChainVizTarget *> activeTargets;
    OwnedArray<ChainVizComponent> activeVizComponents;


    void rebuildComponents();

    void resized() override;

    void sequenceCurrentTimeChanged(Sequence* s, float prevTime, bool skip) override;
};