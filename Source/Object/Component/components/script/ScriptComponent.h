/*
  ==============================================================================

    ScriptComponent.h
    Created: 28 Sep 2020 9:07:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ObjectComponent.h"

class ScriptComponent :
    public ObjectComponent
{
public:
    ScriptComponent(Object* o, var params);
    ~ScriptComponent();

    String getTypeString() const override { return "Script"; }
    static ScriptComponent* create(Object * o, var params) { return new ScriptComponent(o, params); }
};