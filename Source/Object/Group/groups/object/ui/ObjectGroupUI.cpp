/*
  ==============================================================================

    ObjectGroupUI.cpp
    Created: 6 Oct 2020 6:49:00pm
    Author:  bkupe

  ==============================================================================
*/

ObjectTargetEditor::ObjectTargetEditor(ObjectTarget* o, bool isRoot) :
    BaseItemEditor(o, isRoot),
    objectTarget(o)
{
    targetUI.reset(o->target->createTargetUI());
    addAndMakeVisible(targetUI.get());
}

ObjectTargetEditor::~ObjectTargetEditor()
{
}

void ObjectTargetEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    targetUI->setBounds(r.removeFromRight(200));
}
