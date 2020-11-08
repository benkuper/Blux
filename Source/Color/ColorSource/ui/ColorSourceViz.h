/*
  ==============================================================================

    ColorSourceViz.h
    Created: 8 Nov 2020 10:01:59am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Object/Component/components/color/ColorComponent.h"

class ColorSourceViz :
    public InspectableEditor,
    public Timer
{
public:
    ColorSourceViz(ColorComponent* comp); //replace with ColorProvider 2 children classes, one for COmponent and one for colorSource
    ~ColorSourceViz();
    
    ColorComponent* comp;

    void paint(Graphics& g) override;

    void timerCallback() override;

};