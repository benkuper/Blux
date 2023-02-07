/*
  ==============================================================================

    VideoColorSource.cpp
    Created: 2 Nov 2020 6:20:48pm
    Author:  bkupe

  ==============================================================================
*/

PixelMapColorSource::PixelMapColorSource(const String &name, var params) :
    TimedColorSource(name, params)
{
    brightness = sourceParams.addFloatParameter("Brightness", "Brightness to multiply the source pixel to", 1, 0, 1);
    saturation = sourceParams.addFloatParameter("Saturation", "Saturation to multiply the source pixel to", 1, 0, 1);
    hue = sourceParams.addFloatParameter("Hue", "Hue shift to apply after grabbing the pixel color", 0, 0, 1);

}

PixelMapColorSource::~PixelMapColorSource()
{
}

void PixelMapColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time, float originalTime)
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
