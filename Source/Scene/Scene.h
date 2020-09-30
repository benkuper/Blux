/*
  ==============================================================================

    Scene.h
    Created: 26 Sep 2020 1:50:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Scene :
    public BaseItem
{
public:
    Scene(const String & name = "Scene");
    virtual ~Scene();

    var data;

    String getTypeString() const { return "Scene"; }
    static Scene* create(var params) { return new Scene(); }
};