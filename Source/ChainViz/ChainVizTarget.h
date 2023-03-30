/*
  ==============================================================================

    ChainVizTarget.h
    Created: 31 Oct 2020 10:47:53am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/CommonClasses/ComponentType.h"

class ChainVizComponent;
class Object;

class ChainVizTarget
{
public:
    enum ChainVizType { OBJECT_START, OBJECT_END, OBJECT_EFFECT, SCENE_EFFECT, GROUP_EFFECT, GLOBAL_EFFECT, SEQUENCE_EFFECT, CHAINVIZ_TYPE_MAX };

    virtual ~ChainVizTarget();
    virtual ChainVizComponent* createVizComponent(Object * o, ComponentType ct, ChainVizType type);
};