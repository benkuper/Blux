/*
  ==============================================================================

    ColorSourceLayer.h
    Created: 5 Oct 2020 4:03:13pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSourceLayer :
    public SequenceLayer
{
public:
    ColorSourceLayer(Sequence* s, var params);
    ~ColorSourceLayer();

    LayerBlockManager blocks;

    String getTypeString() const override { return "Color Source"; }
    static ColorSourceLayer* create(Sequence* s, var params) { return new ColorSourceLayer(s, params); }
};