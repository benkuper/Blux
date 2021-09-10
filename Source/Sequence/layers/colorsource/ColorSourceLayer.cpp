/*
  ==============================================================================

    ColorSourceLayer.cpp
    Created: 5 Oct 2020 4:03:13pm
    Author:  bkupe

  ==============================================================================
*/

ColorSourceLayer::ColorSourceLayer(Sequence* s, var params) :
    SequenceLayer(s, "Color Source"),
    blocks(this)
{
}

ColorSourceLayer::~ColorSourceLayer()
{
}
