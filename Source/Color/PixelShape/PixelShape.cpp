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

Vector3D<float> PixelShape::getNormalizedPositionForPixel(int index)
{
    return bounds.getNormalizedPosition(getPositionForPixel(index));
}

void PixelShape::onContainerParameterChangedInternal(Parameter* p)
{
    BaseItem::onContainerParameterChangedInternal(p);
    updateBounds();
}


Vector3D<float> PixelShape::Bounds3D::getNormalizedPosition(const Vector3D<float>& absolutePos)
{
    float tx = minPos.x == maxPos.x ? .5f : jmap(absolutePos.x, minPos.x, maxPos.x, 0.f, 1.f);
    float ty = minPos.y == maxPos.y ? .5f: jmap(absolutePos.y, minPos.y, maxPos.y, 0.f, 1.f);
    float tz = minPos.z == maxPos.z ? 0 : jmap(absolutePos.z, minPos.z, maxPos.z, 0.f, 1.f);
    return Vector3D<float>(tx, ty, tz);
}


//---------------

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

void LinePixelShape::updateBounds()
{
    bounds.minPos = Vector3D<float>(jmin(start->x, end->x), jmin(start->y, end->y), jmin(start->z, end->z));
    bounds.maxPos = Vector3D<float>(jmax(start->x, end->x), jmax(start->y, end->y), jmax(start->z, end->z));
}

CirclePixelShape::CirclePixelShape(int resolution) :
    PixelShape("Circle", resolution)
{
    center = addPoint3DParameter("Center", "Center of the circle");
    axis = addPoint3DParameter("Axis", "Axis of the circle");
    radius = addFloatParameter("Radius", "Radius of the circle", .5f, 0);
    startAngle = addFloatParameter("Start Angle", "The angle offset at which to position the first pixel", 0, 0, 360);


    center->setVector(0, 0, 0);
    axis->setVector(0, 0, 1);
}

CirclePixelShape::~CirclePixelShape()
{
}

Vector3D<float> CirclePixelShape::getPositionForPixel(int index)
{
    float angle = (index * 1.0f / resolution) * float_Pi * 2;
    angle += startAngle->floatValue() * float_Pi / 180.f;

    //not using axis for now
    Vector3D<float> pos = center->getVector() + Vector3D<float>(cosf(angle), sinf(angle), 0) * radius->floatValue();
    return pos;
}

void CirclePixelShape::updateBounds()
{
    float rad = radius->floatValue();

    //not using axis for now
    bounds.minPos = center->getVector() - Vector3D<float>(rad, rad, 0);
    bounds.maxPos = center->getVector() + Vector3D<float>(rad, rad, 0);
}
