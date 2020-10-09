/*
  ==============================================================================

    LayoutFilter.h
    Created: 6 Oct 2020 11:32:40pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Filter.h"

class LayoutFilter :
    public Filter
{
public:
    LayoutFilter ();
    ~LayoutFilter ();

    enum LayoutMode { RADIUS, AXIS_X, AXIS_Y, AXIS_Z};
    BoolParameter* enableInView;
    ColorParameter* colorInView;

    TargetParameter* layout;
    EnumParameter* mode;
    Point3DParameter* position;
    FloatParameter* size;
    Automation fadeCurve;

    void clearItem() override;

    virtual FilterResult getFilteredResultForComponentInternal(Object* o, ObjectComponent* c) override;

    String getTypeString() const override { return "Layout Filter"; }
    static LayoutFilter * create(var params) { return new LayoutFilter (); }

    static Array<LayoutFilter*> instances;
    static ChangeBroadcaster broadcaster;
};