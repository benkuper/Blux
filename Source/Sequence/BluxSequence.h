/*
  ==============================================================================

    BluxSequence.h
    Created: 5 Oct 2020 2:42:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
class Object;
class ObjectComponent;

class BluxSequence :
    public Sequence
{
public:
    BluxSequence();
    ~BluxSequence();

    bool manualStartAtLoad;

    virtual void processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier = 1.0f);

    void endLoadFile() override;

    InspectableEditor* getEditor(bool isRoot) override;
    String getTypeString() const override { return "Blux Sequence"; }
};