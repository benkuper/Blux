/*
  ==============================================================================

    EffectManagerEditor.cpp
    Created: 30 Sep 2020 12:39:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

 EffectManagerEditor::EffectManagerEditor(EffectManager* manager, bool isRoot) : 
     GenericManagerEditor(manager, isRoot)
{
     weightUI.reset(manager->globalWeight->createSlider());
     addAndMakeVisible(weightUI.get());
}

 void EffectManagerEditor::resizedInternalHeader(Rectangle<int>& r)
 {
     GenericManagerEditor::resizedInternalHeader(r);
     r.removeFromRight(2);
     r.removeFromLeft(2);
     weightUI->setBounds(r.removeFromRight(100).reduced(2));
 }
