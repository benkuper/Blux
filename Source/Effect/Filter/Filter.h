/*
  ==============================================================================

    Filter.h
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Object/Component/ObjectComponent.h"
#include "Common/ComponentSelector/ComponentSelector.h"

class Object;

class Filter :
    public BaseItem
{
public:
    Filter(const String &name = "Filter");
    virtual ~Filter();

    BoolParameter* useLocalID;

    int getFilteredIDForComponent(Object* o, ObjectComponent * c);
    virtual int getFilteredIDForComponentInternal(Object* o, ObjectComponent* c);
    void saveSceneData(var& sceneData);

};