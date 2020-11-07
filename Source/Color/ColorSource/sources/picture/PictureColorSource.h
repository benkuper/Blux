/*
  ==============================================================================

    PictureColorSource.h
    Created: 2 Nov 2020 6:20:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../../ColorSource.h"

class PictureColorSource :
    public TimedColorSource
{
public:
    PictureColorSource(var params = var());
    ~PictureColorSource();

    FloatParameter* brightness;
    FloatParameter* saturation;
    FloatParameter* hue;
    FileParameter* pictureFile;
    Image picture;

    void onContainerParameterChangedInternal(Parameter*) override;
    virtual void fillColorsForObjectTimeInternal(Array<Colour>& colors, Object* o, ColorComponent* comp, int id, float time) override;

    String getTypeString() const override { return "Picture"; }
    static PictureColorSource* create(var params) { return new PictureColorSource(params); }
};