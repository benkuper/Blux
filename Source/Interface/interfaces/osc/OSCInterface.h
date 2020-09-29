/*
  ==============================================================================

    OSCInterface.h
    Created: 26 Sep 2020 1:52:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Interface.h"

class OSCInterface :
    public Interface
{
public:
    OSCInterface();
    ~OSCInterface();

    String getTypeString() const override { return "OSC"; }
    static OSCInterface* create(var params) { return new OSCInterface(); };
};