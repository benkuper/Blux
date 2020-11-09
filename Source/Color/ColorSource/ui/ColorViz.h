/*
  ==============================================================================

    ColorSourceViz.h
    Created: 8 Nov 2020 10:01:59am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Object/Component/components/color/ColorComponent.h"

class ColorViz :
    public InspectableEditor,
    public Timer
{
public:
    ColorViz(ColorComponent* comp); //replace with ColorProvider 2 children classes, one for Component and one for color source
    ~ColorViz();
    
    ColorComponent* comp;

    void paint(Graphics& g) override;
    void timerCallback() override;

    void visibilityChanged() override;
};