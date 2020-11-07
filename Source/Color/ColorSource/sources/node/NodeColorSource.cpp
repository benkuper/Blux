/*
  ==============================================================================

    NodeColorSource.cpp
    Created: 2 Nov 2020 6:19:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "NodeColorSource.h"

NodeColorSource::NodeColorSource(var params) :
    ColorSource(getTypeString(), params)
{
}

NodeColorSource::~NodeColorSource()
{
}

void NodeColorSource::fillColorsForObject(Array<Colour>& colors, Object* o, ColorComponent* comp, int id, float time)
{
}
