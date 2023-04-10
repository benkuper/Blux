/*
  ==============================================================================

	ColorEffect.h
	Created: 9 Nov 2020 12:53:03pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorComponent;

class ColorEffect :
	public Effect
{
public:
	ColorEffect(const String& name = "ColorEffect", var params = var());
	virtual ~ColorEffect();

	bool fillWithOriginalColors;

	void processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time = -1) override;

	virtual void processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c,int id, float time = -1) {}

};