/*
  ==============================================================================

    ColorSourceViz.cpp
    Created: 8 Nov 2020 10:01:59am
    Author:  bkupe

  ==============================================================================
*/

#include "ColorSourceViz.h"
#include "Color/PixelShape/PixelShape.h"

ColorSourceViz::ColorSourceViz(ColorComponent* comp) :
    InspectableEditor(comp, false),
    comp(comp)
{
    setInterceptsMouseClicks(false, false);
    setSize(150, 150);
    startTimerHz(30);
}

ColorSourceViz::~ColorSourceViz()
{
}

void ColorSourceViz::paint(Graphics& g)
{
    if (inspectable.wasObjectDeleted()) return;
    if (comp->pixelShape == nullptr) return;

    Rectangle<int> r = getLocalBounds().reduced(2);
    float minSize = jmin(r.getWidth(), r.getHeight());
    r = r.withSizeKeepingCentre(minSize, minSize);

    int resolution = comp->resolution->intValue();
    jassert(resolution == comp->colors.size());

    float pixelSize = jmax(jmin(r.getWidth(), r.getHeight()) * 1.0f / resolution, 2.f);

    for (int i = 0; i < resolution; i++)
    {
        Vector3D<float> relPos = comp->pixelShape->getNormalizedPositionForPixel(i);
        Point<int> pos = r.getRelativePoint(relPos.x, 1-relPos.y); //invert Y to have 0 on bottom

        Colour c = comp->colors[i];
        Rectangle<float> pr(pos.x - pixelSize / 2, pos.y - pixelSize / 2, pixelSize, pixelSize);

        g.setColour(c);
        g.fillEllipse(pr);

        g.setColour(c.withAlpha(.5f));
        g.drawEllipse(pr, 1);
    }
}

void ColorSourceViz::timerCallback()
{
    repaint();
}
