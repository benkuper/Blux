/*
  ==============================================================================

    Object.h
    Created: 26 Sep 2020 10:02:32am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Component/ComponentManager.h"

class Object :
    public BaseItem
{
public:
    Object(var params = var());
    virtual ~Object();

    String objectType;
    var objectData;
    ComponentManager componentManager;

    //ui
    File customThumbnailPath;

    String getTypeString() const override { return objectType; }
    
    static Object* create(var params) { return new Object(params); }
};