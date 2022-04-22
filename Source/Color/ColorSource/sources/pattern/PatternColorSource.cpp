/*
  ==============================================================================

    PatternColorSource.cpp
    Created: 2 Nov 2020 6:20:15pm
    Author:  bkupe

  ==============================================================================
*/

SolidColorSource::SolidColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
    speed->defaultValue = 0;
    speed->resetValue();

    sourceColor = addColorParameter("Source Color", "The color to apply. The hue of this color may be modified by the speed and offset parameters.", Colours::red);
}

SolidColorSource::~SolidColorSource()
{
}

void SolidColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    Colour c = sourceColor->getColor().withRotatedHue(time);
    colors.fill(c);
}


//---------------------

RainbowColorSource::RainbowColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
    brightness = addFloatParameter("Brightness", "Brightness of the rainbow", .75f, 0, 1);
    saturation = addFloatParameter("Saturation", "Saturation", 1, 0, 1);
    offset = addFloatParameter("Offset", "The offset of the rainbow, in cycles", 0);
    density = addFloatParameter("Density", "The cycle density of the rainbow", 1);
}

RainbowColorSource::~RainbowColorSource()
{
}

void RainbowColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    int resolution = colors.size();
    for (int i = 0; i < resolution; i++)
    {
        float rel = fmodf((1 - (i * 1.0f / resolution)) * density->floatValue() + time, 1);
        colors.set(i, Colour::fromHSV(rel, saturation->floatValue(), brightness->floatValue(), 1));
    }
}

//---------------------

StrobeColorSource::StrobeColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
    colorON = addColorParameter("Color ON", "", Colours::white);
    colorOFF = addColorParameter("Color OFF", "", Colours::black);
    onOffBalance = addFloatParameter("On-Off Balance", "The balance between on and off time. 0.5s means equals on and off time. .8 means 80% on time, 20% off time.", .5f, 0, 1);
}

StrobeColorSource::~StrobeColorSource()
{
}

void StrobeColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    Colour c = fmodf(curTime, 1) < onOffBalance->floatValue() ? colorON->getColor() : colorOFF->getColor();
    colors.fill(c);
}

//---------------------

NoiseColorSource::NoiseColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
    brightness = addFloatParameter("Brightness", "", 1, 0, 1);
    frontColor = addColorParameter("Front Color", "", Colours::white);
    bgColor = addColorParameter("Background Color", "", Colours::black);
    balance = addFloatParameter("Balance", "The balance between colors", 0, -1, 1);
    contrast = addFloatParameter("Contrast", "", 3);
    scale = addFloatParameter("Scale", "", 3);

    perlin.reset(new siv::PerlinNoise());
}

NoiseColorSource::~NoiseColorSource()
{
}

void NoiseColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    Colour bColor = bgColor->getColor();
    Colour fColor = frontColor->getColor();

    int resolution = colors.size();
    for (int i = 0; i < resolution; i++)
    {
        float v = (perlin->noise0_1((i * scale->floatValue()) / resolution, time) - .5f) * contrast->floatValue() + .5f + balance->floatValue() * 2;
        colors.set(i, bColor.interpolatedWith(fColor, v).withMultipliedBrightness(brightness->floatValue()));
    }
}


//---------------------

PointColorSource::PointColorSource(var params) :
    ColorSource(getTypeString(), params)
{
    brightness = addFloatParameter("Brightness", "", 1, 0, 1);
    pointColor = addColorParameter("Point Color", "The color of the point", Colours::white);
    bgColor = addColorParameter("Background Color", "The color of the background", Colours::black);
    position = addFloatParameter("Position", "Position of the point", .5f);
    size = addFloatParameter("Size", "Size of the point", .25f, 0);
    fade = addFloatParameter("Fade", "The fading of the point", 1, 0, 1);
    extendNum = addIntParameter("Num Props", "The number of props the point is navigating through", 1, 1);
    invertEvens = addBoolParameter("Invert Evens", "If checked, swap the direction of props with even IDs", false);
    invertOdds = addBoolParameter("Invert Odds", "If checked, swap the direction of props with odd IDs", false);
}

PointColorSource::~PointColorSource()
{
}

void PointColorSource::fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    Colour pColor = pointColor->getColor();
    Colour bColor = bgColor->getColor();

    const int resolution = colors.size();

    //float extendPos = position->floatValue() * extendNum->floatValue();

    float relPos = (position->floatValue() /*extendPos */ - id % extendNum->intValue()) * resolution;
    float relStart = jmax<int>(relPos - (size->floatValue() * resolution / 2.f), 0);
    float relEnd = jmin<int>(relPos + (size->floatValue() * resolution / 2.f), resolution);
    float relSize = size->floatValue() * resolution * extendNum->intValue();

    colors.fill(bColor);

    for (int i = relStart; i <= relEnd && i < resolution; i++)
    {
        float diff = 1 - (fabsf(i - relPos) * 1.f / (relSize / (fade->floatValue() * 2 * extendNum->intValue())));
        bool invert = id % 2 == 0 ? invertEvens->boolValue() : invertOdds->boolValue();
        Colour c = bColor.interpolatedWith(pColor, diff);
        colors.set(invert ? resolution - i : i, c.withMultipliedBrightness(brightness->floatValue()));
    }
}


//---------------------

MultiPointColorSource::MultiPointColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
    brightness = addFloatParameter("Brightness", "", 1, 0, 1);
    pointColor = addColorParameter("Color", "The color of the point", Colours::white);
    bgColor = addColorParameter("Background Color", "The color of the background", Colours::black);
    gap = addFloatParameter("Gap", "The gap between lines per prop", .25f, 0, 1);
    size = addFloatParameter("Size", "Size of the point, relative to the gap", .5f, 0, 1);
    fade = addFloatParameter("Fade", "The fading of the point", 1, 0, 1);
}

MultiPointColorSource::~MultiPointColorSource()
{
}

void MultiPointColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    const int resolution = colors.size();

    Colour bColor = bgColor->getColor();
    Colour pColor = pointColor->getColor();

    colors.fill(bColor);
    if (gap->floatValue() == 0 || size->floatValue() == 0) return;

    float targetPos = time;
    if (targetPos < 0) targetPos = fmodf(targetPos, -gap->floatValue()) + gap->floatValue();

    for (int i = 0; i < resolution; i++)
    {
        float relTotal = fmodf((1 - (i * 1.0f / resolution)), 1);
        float relGap = fmodf((relTotal + gap->floatValue() + targetPos) / gap->floatValue(), 1);
        float relCentered = 1 - fabsf((relGap - .5f) * 2) * 1 / size->floatValue();

        if (relCentered < 0) continue;

        float relFadedVal = jmap<float>(jlimit<float>(0, 1, relCentered), 1 - fade->floatValue(), 1);

        Colour c = bColor.interpolatedWith(pColor, relFadedVal);
        colors.set(i, c.withMultipliedBrightness(brightness->floatValue()));
    }
}

GradientColorSource::GradientColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
    brightness = addFloatParameter("Brightness", "", 1, 0, 1);
    density = addFloatParameter("Density", "The cycle density of the rainbow", 1);

    gradient.reset(new GradientColorManager(1, true));
    gradient->setAllowKeysOutside(false);
    addChildControllableContainer(gradient.get());
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

void GradientColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    const int resolution = colors.size();

    for (int i = 0; i < resolution; i++)
    {
        float p = fmodf(time + i * density->floatValue() / resolution, 1);
        if (p < 0) p++;
        colors.set(i, gradientTarget->getColorForPosition(p).withMultipliedBrightness(brightness->floatValue()));
    }
}

void GradientColorSource::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
    TimedColorSource::onControllableFeedbackUpdateInternal(cc, c);

    if (gradient != nullptr && c == gradient->enabled)
    {
        gradientTarget = gradient->enabled->boolValue()?gradient.get():((GradientColorSource*)sourceTemplate)->gradient.get();
    }
}
