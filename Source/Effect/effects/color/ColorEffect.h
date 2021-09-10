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

	virtual var getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time = -1);
	virtual void processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c,int id, float time = -1) {}

};