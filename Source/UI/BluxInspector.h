/*
  ==============================================================================

    BluxInspector.h
    Created: 9 Nov 2020 8:12:06am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ColorViz;

class BluxInspector :
    public Inspector
{
public:
    BluxInspector(InspectableSelectionManager* s);
    ~BluxInspector();

    std::unique_ptr<ColorViz> colorViz;

    virtual void setCurrentInspectables(Array<Inspectable*>inspectables = Array<Inspectable*>(), bool setInspectableSelection = true);
    
    void paint(Graphics& g) override;
    void resizedInternal(Rectangle<int>& r) override;
};