/*
  ==============================================================================

    BluxSequenceChainVizUI.cpp
    Created: 1 Nov 2020 11:10:13am
    Author:  bkupe

  ==============================================================================
*/

BluxSequenceChainVizUI::BluxSequenceChainVizUI(BluxSequence* s, Object* o, ChainVizTarget::ChainVizType type) :
    BaseItemChainVizComponent(s, o, type),
    sequence(s),
    inspectable(s)
{
    transparentBG = true;
    sequence->addSequenceListener(this);
    activeTargets = sequence->getChainVizTargetsForObject(object);
    rebuildComponents();
}

BluxSequenceChainVizUI::~BluxSequenceChainVizUI()
{
    if(!inspectable.wasObjectDeleted()) sequence->removeSequenceListener(this);
}

void BluxSequenceChainVizUI::rebuildComponents()
{
    int tw = 0;
    const int gap = 10;

    MessageManagerLock mmLock;
    
    for (auto& c : activeVizComponents) removeChildComponent(c);
    activeVizComponents.clear();

    for (auto& t : activeTargets)
    {
        ChainVizComponent* c = t->createVizComponent(object, type);
        addAndMakeVisible(c);
        activeVizComponents.add(c);
        tw += c->getWidth() + gap;
    }

    showItemName = activeVizComponents.isEmpty();
    setSize(jmax(tw - gap, 0), getHeight());

}

void BluxSequenceChainVizUI::resized()
{
    BaseItemChainVizComponent::resized();

    const int gap = 10;
    Rectangle<int> r = getLocalBounds();
    for (auto& c : activeVizComponents)
    {
        c->setBounds(r.removeFromLeft(c->getWidth()));
        r.removeFromLeft(gap);
    }
}

void BluxSequenceChainVizUI::sequenceCurrentTimeChanged(Sequence* s, float prevTime, bool skip)
{
    Array<ChainVizTarget*> newTargets = sequence->getChainVizTargetsForObject(object);
    if (activeTargets != newTargets)
    {
        activeTargets = newTargets;
        rebuildComponents();
    }
}
