/*
  ==============================================================================

    ScriptEffect.h
    Created: 20 Nov 2021 10:06:39am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Script;

class ScriptEffect :
    public ColorEffect
{
public:
    ScriptEffect(var params = var());
    virtual ~ScriptEffect();

    std::unique_ptr<Script> script;
    const Identifier updateColorsIdentifier = "updateColors";

    void processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1) override;

    DECLARE_TYPE("Script Effect")
};
