/*
  ==============================================================================

    VideoColorSource.cpp
    Created: 2 Nov 2020 6:20:48pm
    Author:  bkupe

  ==============================================================================
*/

#include "VideoColorSource.h"
#include "Object/Component/components/color/ColorComponent.h"
#include "Color/PixelShape/PixelShape.h"

PixelMapColorSource::PixelMapColorSource(const String &name, var params) :
    TimedColorSource(name, params)
{
    brightness = addFloatParameter("Brightness", "Brightness to multiply the source pixel to", 1, 0, 1);
    saturation = addFloatParameter("Saturation", "Saturation to multiply the source pixel to", 1, 0, 1);
    hue = addFloatParameter("Hue", "Hue shift to apply after grabbing the pixel color", 0, 0, 1);

}

PixelMapColorSource::~PixelMapColorSource()
{
}

void PixelMapColorSource::fillColorsForObjectTimeInternal(Array<Colour>& colors, Object* o, ColorComponent* comp, int id, float time)
{
    if (!sourceImage.isValid()) return;

    PixelShape* shape = comp->pixelShape.get();
    const int resolution = colors.size();
    for (int i = 0; i < resolution; i++)
    {
        Vector3D<float> p = shape->getPositionForPixel(i);
        int tx = p.x * sourceImage.getWidth();
        int ty = p.y * sourceImage.getHeight();
        if (tx < 0 || tx >= sourceImage.getWidth() || ty < 0 || ty >= sourceImage.getHeight()) continue;


        Colour col = sourceImage.getPixelAt(tx, ty);
        colors.set(i, col);
    }
}

VideoColorSource::VideoColorSource(var params) :
    PixelMapColorSource(getTypeString(), params)
{
}

VideoColorSource::~VideoColorSource()
{
}

SpoutSyphonColorSource::SpoutSyphonColorSource(var params)
{
}

SpoutSyphonColorSource::~SpoutSyphonColorSource()
{
}
