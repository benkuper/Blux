/*
  ==============================================================================

    PixelShape.h
    Created: 2 Nov 2020 6:19:16pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class PixelShape :
    public BaseItem
{
public:
    PixelShape(const String &name = "PixelShape", int resolution = 1);
    virtual ~PixelShape();

    int resolution;
    bool needsSquareRatio;//for ui

    struct Bounds3D
    {
        Vector3D<float> minPos;
        Vector3D<float> maxPos;
        Vector3D<float> getNormalizedPosition(const Vector3D<float>& absolutePos);
    };
    Bounds3D bounds;


    virtual Vector3D<float> getPositionForPixel(int index);
    virtual Vector3D<float>  getNormalizedPositionForPixel(int index);
    virtual void updateBounds() {}

    virtual void onContainerParameterChangedInternal(Parameter* p) override;
};

class PointPixelShape :
    public PixelShape
{
public:
    PointPixelShape(int resolution) : PixelShape(getTypeString(), resolution) {}
    ~PointPixelShape() {}

    String getTypeString() const override { return "Point"; }
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
    virtual void updateBounds() override;

    String getTypeString() const override { return "Line"; }
};

class CirclePixelShape :
    public PixelShape
{
public:
    CirclePixelShape(int resolution = 1);
    virtual ~CirclePixelShape();

    Point3DParameter* center;
    Point3DParameter* axis;
    FloatParameter* radius;
    FloatParameter* startAngle;

    virtual Vector3D<float>  getPositionForPixel(int index) override;
    virtual void updateBounds() override;

    String getTypeString() const override { return "Circle"; }
};