/*
  ==============================================================================

    GroupManagerUI.h
    Created: 29 Sep 2020 9:46:32pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../GroupManager.h"
#include "GroupUI.h"

class GroupManagerUI :
    public BaseManagerShapeShifterUI<GroupManager, Group, GroupUI>
{
public:
    GroupManagerUI(const String &name);
    ~GroupManagerUI();


    static GroupManagerUI* create(const String& name) { return new GroupManagerUI(name); }
};