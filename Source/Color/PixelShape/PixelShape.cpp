/*
  ==============================================================================

    PixelShape.cpp
    Created: 2 Nov 2020 6:19:16pm
    Author:  bkupe

  ==============================================================================
*/

#include "PixelShape.h"

PixelShape::PixelShape(const String& name, int resolution) :
    BaseItem(name, false, false),
    resolution(resolution)
{
}

PixelShape::~PixelShape()
{
}

Vector3D<float> PixelShape::getPositionForPixel(int index)
{
    return Vector3D<float>();
}

LinePixelShape::LinePixelShape(int resolution) :
    PixelShape("Line", resolution)
{
    start = addPoint3DParameter("Start", "Start of the line");
    end = addPoint3DParameter("End", "End of the line");
    start->setVector(-1, 0, 0);
    end->setVector(1, 0, 0);
}

LinePixelShape::~LinePixelShape()
{
}

Vector3D<float> LinePixelShape::getPositionForPixel(int index)
{
    float p = index * 1.0f / resolution;
    return start->getVector() + (end->getVector() - start->getVector()) * p;
}
