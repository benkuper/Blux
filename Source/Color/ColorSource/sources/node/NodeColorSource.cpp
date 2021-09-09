/*
  ==============================================================================

    NodeColorSource.cpp
    Created: 2 Nov 2020 6:19:59pm
    Author:  bkupe

  ==============================================================================
*/

NodeColorSource::NodeColorSource(var params) :
    ColorSource(getTypeString(), params)
{
}

NodeColorSource::~NodeColorSource()
{
}

void NodeColorSource::fillColorsForObjectInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
}
