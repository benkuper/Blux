/*
  ==============================================================================

    BluxSequenceEditor.cpp
    Created: 10 Oct 2020 10:21:57am
    Author:  bkupe

  ==============================================================================
*/

#include "BluxSequenceEditor.h"

BluxSequenceEditor::BluxSequenceEditor(BluxSequence* s, bool isRoot) :
    BaseItemEditor(s, isRoot),
    editBT("Edit"),
    bluxSequence(s)
{
    addAndMakeVisible(&editBT);
    editBT.addListener(this);
}

BluxSequenceEditor::~BluxSequenceEditor()
{
}

void BluxSequenceEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    editBT.setBounds(r.removeFromRight(50).reduced(2));
}

void BluxSequenceEditor::buttonClicked(Button* b)
{
    BaseItemEditor::buttonClicked(b);
    if (b == &editBT)
    {
        if (TimeMachineView* v = ShapeShifterManager::getInstance()->getContentForType<TimeMachineView>())
        {
            v->setSequence(bluxSequence);
        }
        
    }
}
