/*
  ==============================================================================

    ObjectChainVizUI.h
    Created: 30 Oct 2020 4:35:09pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ChainViz/ChainViz.h"
class Object;

class ObjectChainVizUI :
    public BaseItemChainVizComponent
{
public:
    ObjectChainVizUI(Object* o, ChainVizTarget::ChainVizType type);
    ~ObjectChainVizUI();

    std::unique_ptr<FloatSliderUI> intensityUI;
  
    void resized() override;
};