/*
  ==============================================================================

    ScriptColorSource.h
    Created: 2 Nov 2020 6:20:28pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ScriptColorSource :
    public TimedColorSource
{
public:
    ScriptColorSource(var params = var());
    ~ScriptColorSource();

    //NodeManager nodeManager;

    virtual void fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime) override;

    String getTypeString() const override { return "Script"; }
    static ScriptColorSource* create(var params) { return new ScriptColorSource(params); }
};