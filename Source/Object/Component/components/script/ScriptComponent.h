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
    ScriptComponent(var params);
    ~ScriptComponent();

    String getTypeString() const override { return "Script"; }
    static ScriptComponent* create(var params) { return new ScriptComponent(params); }
};