/*
  ==============================================================================

    ColorViz.cpp
    Created: 8 Nov 2020 10:01:59am
    Author:  bkupe

  ==============================================================================
*/

#include "Color/ColorIncludes.h"

ColorViz::ColorViz(ColorComponent* comp) :
    InspectableEditor(comp, false),
    comp(comp)
{
    setInterceptsMouseClicks(false, false);
    setSize(150, 150);
    startTimerHz(30);
}

ColorViz::~ColorViz()
{
}

void ColorViz::paint(Graphics& g)
{
    if (inspectable.wasObjectDeleted()) return;
    if (comp->pixelShape == nullptr) return;
    if (comp->outColors.isEmpty()) return;

    Rectangle<int> r = getLocalBounds().reduced(2);
    float minSize = jmin(r.getWidth(), r.getHeight());
    if(comp->pixelShape->needsSquareRatio) r = r.withSizeKeepingCentre(minSize, minSize);

    int resolution = comp->resolution->intValue();
    jassert(resolution == comp->outColors.size());

    float pixelSize = jmax(jmin(r.getWidth(), r.getHeight()) * 1.0f / resolution, 2.f);
    
    Rectangle<int> vizR = r.reduced(pixelSize / 2);

    for (int i = 0; i < resolution; i++)
    {
        Vector3D<float> relPos = comp->pixelShape->getNormalizedPositionForPixel(i);
        Point<int> pos = vizR.getRelativePoint(relPos.x, 1-relPos.y); //invert Y to have 0 on bottom

        Colour c = comp->outColors[i];
        Rectangle<float> pr(pos.x - pixelSize / 2, pos.y - pixelSize / 2, pixelSize, pixelSize);

        g.setColour(c);
        g.fillEllipse(pr);

        g.setColour(c.brighter(.8f).withAlpha(.7f));
        g.drawEllipse(pr, 1);
    }
}

void ColorViz::timerCallback()
{
    repaint();
}

void ColorViz::visibilityChanged()
{
    InspectableEditor::visibilityChanged();
    if (!isVisible()) stopTimer();
    else startTimerHz(30);
} 
