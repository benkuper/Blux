/*
  ==============================================================================

	PatternColorSource.cpp
	Created: 2 Nov 2020 6:20:15pm
	Author:  bkupe

  ==============================================================================
*/

#include "Color/ColorIncludes.h"

SolidColorSource::SolidColorSource(var params) :
	TimedColorSource(getTypeString(), params)
{
	speed->setDefaultValue(0);

	sourceColor = sourceParams.addColorParameter("Source Color", "The color to apply. The hue of this color may be modified by the speed and offset parameters.", Colours::black);
}

SolidColorSource::~SolidColorSource()
{
}

void SolidColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime)
{
	colors.fill(GetLinkedColor(sourceColor).withRotatedHue(time == originalTime ? time : 0));
}


//---------------------

RainbowColorSource::RainbowColorSource(var params) :
	TimedColorSource(getTypeString(), params)
{
	brightness = sourceParams.addFloatParameter("Brightness", "Brightness of the rainbow", .75f, 0, 1);
	saturation = sourceParams.addFloatParameter("Saturation", "Saturation", 1, 0, 1);
	offset = sourceParams.addFloatParameter("Offset", "The offset of the rainbow, in cycles", 0);
	density = sourceParams.addFloatParameter("Density", "The cycle density of the rainbow", 1);
}

RainbowColorSource::~RainbowColorSource()
{
}

void RainbowColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime)
{
	int resolution = colors.size();
	for (int i = 0; i < resolution; i++)
	{
		double rel = fmodf((1 - (i * 1.0f / resolution)) * (double)GetSourceLinkedValue(density) + time, 1);
		colors.set(i, Colour::fromHSV(rel, (double)GetSourceLinkedValue(saturation), (double)GetSourceLinkedValue(brightness), 1));
	}
}

//---------------------

StrobeColorSource::StrobeColorSource(var params) :
	TimedColorSource(getTypeString(), params)
{
	colorON = sourceParams.addColorParameter("Color ON", "", Colours::white);
	colorOFF = sourceParams.addColorParameter("Color OFF", "", Colours::black);
	onOffBalance = sourceParams.addFloatParameter("On-Off Balance", "The balance between on and off time. 0.5s means equals on and off time. .8 means 80% on time, 20% off time.", .5f, 0, 1);
}

StrobeColorSource::~StrobeColorSource()
{
}

void StrobeColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime)
{
	Colour c = fmodf(time, 1) < (double)GetSourceLinkedValue(onOffBalance) ? GetLinkedColor(colorON) : GetLinkedColor(colorOFF);
	colors.fill(c);
}

//---------------------

NoiseColorSource::NoiseColorSource(var params) :
	TimedColorSource(getTypeString(), params)
{
	brightness = sourceParams.addFloatParameter("Brightness", "", 1, 0, 1);
	frontColor = sourceParams.addColorParameter("Front Color", "", Colours::white);
	bgColor = sourceParams.addColorParameter("Background Color", "", Colours::black);
	balance = sourceParams.addFloatParameter("Balance", "The balance between colors", 0, -1, 1);
	contrast = sourceParams.addFloatParameter("Contrast", "", 3);
	scale = sourceParams.addFloatParameter("Scale", "", 3);

	perlin.reset(new siv::PerlinNoise());
}

NoiseColorSource::~NoiseColorSource()
{
}

void NoiseColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime)
{
	Colour bColor = GetLinkedColor(bgColor);
	Colour fColor = GetLinkedColor(frontColor);

	int resolution = colors.size();
	for (int i = 0; i < resolution; i++)
	{
		double v = (perlin->noise0_1((i * (double)GetSourceLinkedValue(scale)) / resolution, time) - .5f) * (double)GetSourceLinkedValue(contrast) + .5f + (double)GetSourceLinkedValue(balance) * 2;
		colors.set(i, bColor.interpolatedWith(fColor, v).withMultipliedBrightness((double)GetSourceLinkedValue(brightness)));
	}
}


//---------------------

PointColorSource::PointColorSource(var params) :
	ColorSource(getTypeString(), params)
{
	brightness = sourceParams.addFloatParameter("Brightness", "", 1, 0, 1);
	pointColor = sourceParams.addColorParameter("Point Color", "The color of the point", Colours::white);
	bgColor = sourceParams.addColorParameter("Background Color", "The color of the background", Colours::black);
	position = sourceParams.addFloatParameter("Position", "Position of the point", .5f);
	size = sourceParams.addFloatParameter("Size", "Size of the point", .25f, 0);
	fade = sourceParams.addFloatParameter("Fade", "The fading of the point", 1, 0, 1);
	extendNum = sourceParams.addIntParameter("Num Props", "The number of props the point is navigating through", 1, 1);
	invertEvens = sourceParams.addBoolParameter("Invert Evens", "If checked, swap the direction of props with even IDs", false);
	invertOdds = sourceParams.addBoolParameter("Invert Odds", "If checked, swap the direction of props with odd IDs", false);
}

PointColorSource::~PointColorSource()
{
}

void PointColorSource::fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float  originalTime)
{
	Colour pColor = GetLinkedColor(pointColor);
	Colour bColor = GetLinkedColor(bgColor);

	const int resolution = colors.size();

	double sizeVal = GetSourceLinkedValue(size);
	int extendVal = GetSourceLinkedValue(extendNum);

	//double extendPos = position->doubleValue() * extendNum->doubleValue();

	double relPos = ((double)GetSourceLinkedValue(position)/*extendPos */ - id % extendVal) * resolution;
	double relStart = jmax<int>(relPos - (sizeVal * resolution / 2.f), 0);
	double relEnd = jmin<int>(relPos + (sizeVal * resolution / 2.f), resolution);
	double relSize = sizeVal * resolution * extendVal;

	colors.fill(bColor);

	for (int i = relStart; i <= relEnd && i < resolution; i++)
	{
		double diff = 1 - (fabsf(i - relPos) * 1.f / (relSize / ((double)GetSourceLinkedValue(fade) * 2 * extendVal)));

		bool invert = id % 2 == 0 ? GetSourceLinkedValue(invertEvens) : GetSourceLinkedValue(invertOdds);
		Colour c = bColor.interpolatedWith(pColor, diff);
		colors.set(invert ? resolution - i : i, c.withMultipliedBrightness((double)GetSourceLinkedValue(brightness)));
	}
}


//---------------------

MultiPointColorSource::MultiPointColorSource(var params) :
	TimedColorSource(getTypeString(), params)
{
	brightness = sourceParams.addFloatParameter("Brightness", "", 1, 0, 1);
	pointColor = sourceParams.addColorParameter("Color", "The color of the point", Colours::white);
	bgColor = sourceParams.addColorParameter("Background Color", "The color of the background", Colours::black);
	gap = sourceParams.addFloatParameter("Gap", "The gap between lines per prop", .25f, 0, 1);
	size = sourceParams.addFloatParameter("Size", "Size of the point, relative to the gap", .5f, 0, 1);
	fade = sourceParams.addFloatParameter("Fade", "The fading of the point", 1, 0, 1);
}

MultiPointColorSource::~MultiPointColorSource()
{
}

void MultiPointColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime)
{
	const int resolution = colors.size();

	Colour bColor = GetLinkedColor(bgColor);
	Colour pColor = GetLinkedColor(pointColor);

	colors.fill(bColor);
	double gapVal = GetSourceLinkedValue(gap);

	if (gapVal == 0 || size->doubleValue() == 0) return;

	double targetPos = time;
	if (targetPos < 0) targetPos = fmodf(targetPos, -gapVal) + gapVal;

	for (int i = 0; i < resolution; i++)
	{
		double relTotal = fmodf((1 - (i * 1.0f / resolution)), 1);
		double relGap = fmodf((relTotal + gapVal + targetPos) / gapVal, 1);
		double relCentered = 1 - fabsf((relGap - .5f) * 2) * 1 / (double)GetSourceLinkedValue(size);

		if (relCentered < 0) continue;

		double relFadedVal = jmap<double>(jlimit<double>(0, 1, relCentered), 1 - (double)GetSourceLinkedValue(fade), 1);

		Colour c = bColor.interpolatedWith(pColor, relFadedVal);
		colors.set(i, c.withMultipliedBrightness((double)GetSourceLinkedValue(brightness)));
	}
}

GradientColorSource::GradientColorSource(var params) :
	TimedColorSource(getTypeString(), params)
{
	brightness = sourceParams.addFloatParameter("Brightness", "", 1, 0, 1);
	density = sourceParams.addFloatParameter("Density", "The cycle density of the rainbow", 1);

	gradient.reset(new GradientColorManager(1, true));
	gradient->setAllowKeysOutside(false);
	sourceParams.addChildControllableContainer(gradient.get());
	gradientTarget = gradient.get();
}

GradientColorSource::~GradientColorSource()
{

}

void GradientColorSource::linkToTemplate(ColorSource* sourceTemplate)
{
	TimedColorSource::linkToTemplate(sourceTemplate);
	if (sourceTemplate != nullptr)
	{
		gradient->setCanBeDisabled(true);
		gradient->hideEditorHeader = false;
		gradient->editorCanBeCollapsed = true;
		gradient->editorIsCollapsed = true;
		gradient->enabled->setValue(false);
		gradientTarget = ((GradientColorSource*)sourceTemplate)->gradient.get();
		gradient->loadJSONData(gradientTarget->getJSONData());
	}
	else
	{
		gradient->setCanBeDisabled(false);
		gradientTarget = gradient.get();
	}
}

void GradientColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime)
{
	const int resolution = colors.size();

	for (int i = 0; i < resolution; i++)
	{
		double p = fmodf(time + i * (double)GetSourceLinkedValue(density) / resolution, 1);
		if (p < 0) p++;
		colors.set(i, gradientTarget->getColorForPosition(p).withMultipliedBrightness((double)GetSourceLinkedValue(brightness)));
	}
}

void GradientColorSource::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	TimedColorSource::onControllableFeedbackUpdateInternal(cc, c);

	if (gradient != nullptr && c == gradient->enabled)
	{
		gradientTarget = gradient->enabled->boolValue() ? gradient.get() : ((GradientColorSource*)sourceTemplate)->gradient.get();
	}
}
