/*
  ==============================================================================

    GroupUI.h
    Created: 29 Sep 2020 9:46:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Group.h"

class GroupUI :
    public BaseItemUI<Group>
{
public:
    GroupUI(Group * item);
    ~GroupUI();


    void itemDropped(const DragAndDropTarget::SourceDetails& details) override;
};