/*
  ==============================================================================

	ColorGradientMapEffect.h
	Created: 2 Nov 2020 6:25:49pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ColorEffect.h"

class GradientRemapEffect :
	public ColorEffect
{
public:
	GradientRemapEffect(var params);
	~GradientRemapEffect();

	enum SourceChannel { BRIGHTNESS, HUE, SATURATION, RED, GREEN, BLUE, ALPHA, CHANNEL_MAX };
	const String channelNames[CHANNEL_MAX]{ "Brightness", "Hue", "Saturation", "Red", "Green", "Blue", "Alpha" };

	EnumParameter* sourceChannel;
	GradientColorManager gradient;

	void processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1);

	String getTypeString() const override { return getTypeStringStatic(); }
	const static String getTypeStringStatic() { return "Gradient Remap"; }
	static GradientRemapEffect* create(var params) { return new GradientRemapEffect(params); }
};