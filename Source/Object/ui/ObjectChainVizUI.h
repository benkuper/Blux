/*
  ==============================================================================

    ObjectChainVizUI.h
    Created: 30 Oct 2020 4:35:09pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;

class ObjectChainVizUI :
    public BaseItemChainVizComponent
{
public:
    ObjectChainVizUI(Object* o, ComponentType ct, ChainVizTarget::ChainVizType type);
    ~ObjectChainVizUI();

    std::unique_ptr<ParameterUI> paramUI;
  
    void resized() override;
};