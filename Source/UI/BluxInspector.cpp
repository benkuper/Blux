/*
  ==============================================================================

    BluxInspector.cpp
    Created: 9 Nov 2020 8:12:06am
    Author:  bkupe

  ==============================================================================
*/

#include "BluxInspector.h"
#include "Object/ObjectIncludes.h"
#include "Color/ColorIncludes.h"

BluxInspector::BluxInspector(InspectableSelectionManager* s) :
    Inspector(s)
{
}

BluxInspector::~BluxInspector()
{
}

void BluxInspector::setCurrentInspectables(Array<Inspectable*> inspectables, bool setInspectableSelection)
{

    Inspector::setCurrentInspectables(inspectables, setInspectableSelection);

}

void BluxInspector::paint(Graphics& g)
{
    Inspector::paint(g);


   
}

void BluxInspector::resizedInternal(Rectangle<int>& r)
{

    Inspector::resizedInternal(r);
}
