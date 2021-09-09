/*
  ==============================================================================

    ObjectGroupUI.h
    Created: 6 Oct 2020 6:49:00pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ObjectGroup.h"

class ObjectTargetEditor :
    public BaseItemEditor
{
public:
    ObjectTargetEditor(ObjectTarget* o, bool isRoot);
    ~ObjectTargetEditor();

    ObjectTarget* objectTarget;

    std::unique_ptr<TargetParameterUI> targetUI;

    void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
};
    