/*
  ==============================================================================

	ColorGradientMapEffect.cpp
	Created: 2 Nov 2020 6:25:49pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

GradientRemapEffect::GradientRemapEffect(var params) :
	ColorEffect(getTypeString(), params),
	gradient(1)
{
	saveAndLoadRecursiveData = true;

	sourceChannel = effectParams.addEnumParameter("Source Channel", "The channel to use as position for the gradient remap");
	for (int i = 0; i < CHANNEL_MAX; i++) sourceChannel->addOption(channelNames[i], (SourceChannel)i);

	gradient.allowKeysOutside = false;

	gradient.addColorAt(0, RED_COLOR);
	gradient.addColorAt(1, GREEN_COLOR);

	addChildControllableContainer(&gradient, false, 0);
}

GradientRemapEffect::~GradientRemapEffect()
{
}

void GradientRemapEffect::processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
	SourceChannel ch = (SourceChannel)(int)GetLinkedValue(sourceChannel);

	int numColors = colors.size();
	for (int i = 0; i < numColors; i++)
	{
		float sourceVal = 0;
		switch (ch)
		{
		case BRIGHTNESS: sourceVal = colors[i].getBrightness(); break;
		case HUE: sourceVal = colors[i].getHue(); break;
		case SATURATION: sourceVal = colors[i].getSaturation(); break;
		case RED: sourceVal = colors[i].getFloatRed(); break;
		case GREEN: sourceVal = colors[i].getFloatGreen(); break;
		case BLUE: sourceVal = colors[i].getFloatBlue(); break;
		case ALPHA: sourceVal = colors[i].getFloatAlpha(); break;
		}

		colors.set(i, gradient.getColorForPosition(sourceVal));
	}
}
