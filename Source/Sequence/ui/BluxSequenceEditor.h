/*
  ==============================================================================

    BluxSequenceEditor.h
    Created: 10 Oct 2020 10:21:57am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class BluxSequenceEditor :
    public BaseItemEditor
{
public:
    BluxSequenceEditor(BluxSequence* s, bool isRoot);
    ~BluxSequenceEditor();

    BluxSequence* bluxSequence;
    TextButton editBT;

    void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

    void buttonClicked(Button* b) override;
};