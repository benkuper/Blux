/*
  ==============================================================================

    GlobalSequenceManagerUI.h
    Created: 5 Oct 2020 2:42:26pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../GlobalSequenceManager.h"

class GlobalSequenceManagerUI :
    public SequenceManagerUI
{
public:
    GlobalSequenceManagerUI(const String& name);
    ~GlobalSequenceManagerUI();

    static GlobalSequenceManagerUI* create(const String& name) { return new GlobalSequenceManagerUI(name); }
};