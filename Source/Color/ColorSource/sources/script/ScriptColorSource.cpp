/*
  ==============================================================================

    ScriptColorSource.cpp
    Created: 2 Nov 2020 6:20:28pm
    Author:  bkupe

  ==============================================================================
*/

ScriptColorSource::ScriptColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
}

ScriptColorSource::~ScriptColorSource()
{
}

void ScriptColorSource::fillColorsForObjectTimeInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* comp, int id, float time)
{
}
