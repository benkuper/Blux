/*
  ==============================================================================

    Viz.h
    Created: 5 Apr 2022 10:41:47am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class VizPanel :
    public ShapeShifterContentComponent
{
public:
    VizPanel(StringRef name);
    ~VizPanel() {}
    WebBrowserComponent web;
    
    void resized() override;

    static VizPanel* create(const String& name) { return new VizPanel(name); }
};