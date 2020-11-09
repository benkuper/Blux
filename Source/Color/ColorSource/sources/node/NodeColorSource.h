/*
  ==============================================================================

    NodeColorSource.h
    Created: 2 Nov 2020 6:19:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../../ColorSource.h"

class NodeColorSource :
    public ColorSource
{
public:
    NodeColorSource(var params = var());
    ~NodeColorSource();

    //NodeManager nodeManager;

    virtual void fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time) override;

    String getTypeString() const override { return "Node"; }
    static NodeColorSource* create(var params) { return new NodeColorSource(params); }
};