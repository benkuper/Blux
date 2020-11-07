/*
  ==============================================================================

    PixelShape.h
    Created: 2 Nov 2020 6:19:16pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PixelShape :
    public BaseItem
{
public:
    PixelShape(const String &name = "PixelShape", int resolution = 1);
    virtual ~PixelShape();

    int resolution;

    virtual Vector3D<float> getPositionForPixel(int index);
};


class LinePixelShape :
    public PixelShape
{
public:
    LinePixelShape(int resolution = 1);
    virtual ~LinePixelShape();

    Point3DParameter* start;
    Point3DParameter* end;

    virtual Vector3D<float>  getPositionForPixel(int index) override;
};