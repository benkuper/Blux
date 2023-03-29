/*
  ==============================================================================

	ColorEffect.cpp
	Created: 9 Nov 2020 12:53:03pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

ColorEffect::ColorEffect(const String& name, var params) :
	Effect(name, params),
	fillWithOriginalColors(true)
{
}

ColorEffect::~ColorEffect()
{
}

void ColorEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	if (c->componentType != COLOR) return;
	ColorComponent* cComp = (ColorComponent*)c;

	int resolution = cComp->resolution->intValue();
	Array<Colour, CriticalSection> targetColors;
	targetColors.resize(resolution);

	var sourceColors = values[nullptr]; // using nullptr to hold colors or whatever is not related to a computed parameter

	if (fillWithOriginalColors)
	{
		for (int i = 0; i < resolution; i++)
		{
			if (i >= sourceColors.size()) targetColors.set(i, Colours::black);
			var col = sourceColors[i];
			targetColors.set(i, Colour::fromFloatRGBA(col[0], col[1], col[2], col[3]));
		}
	}

	if (time == -1) time = Time::getMillisecondCounter() / 1000.0f;
	processedEffectColorsInternal(targetColors, o, (ColorComponent*)c, id, time);

	var result;
	for (auto& col : targetColors)
	{
		var cv;
		cv.append(col.getFloatRed());
		cv.append(col.getFloatGreen());
		cv.append(col.getFloatBlue());
		cv.append(col.getFloatAlpha());
		result.append(cv);
	}

	targetValues.set(nullptr, result);

	//viz
	if (targetColors.size() > 0)
	{
		if (vizParameter != nullptr && !vizParameter.wasObjectDeleted() && vizComputedParamRef != nullptr && vizComputedParamRef == c->mainParameter)
		{
			((ColorParameter*)vizParameter.get())->setColor(targetColors[0]);
		}
	}

}