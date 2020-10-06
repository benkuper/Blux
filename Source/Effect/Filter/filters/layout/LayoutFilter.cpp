/*
  ==============================================================================

    LayoutFilter.cpp
    Created: 6 Oct 2020 11:32:40pm
    Author:  bkupe

  ==============================================================================
*/

#include "LayoutFilter.h"
#include "Object/Object.h"

LayoutFilter::LayoutFilter() :
    Filter(getTypeString()),
    fadeCurve("Fade Curve")
{
    enableInView = addBoolParameter("Enable in view", "If checked, this effect will be visible in the stage layout view", true);
    mode = addEnumParameter("Mode", "What to check for this filter");
    mode->addOption("Radius", RADIUS)->addOption("X", AXIS_X)->addOption("Y", AXIS_Y)->addOption("Z", AXIS_Z);
    position = addPoint3DParameter("Position", "The position to filter against");
    size = addFloatParameter("Size", "Size for this mode", 1);
    
    fadeCurve.isSelectable = false;
    fadeCurve.length->setValue(1);
    fadeCurve.addKey(1, 0, false);
    fadeCurve.items[0]->easingType->setValueWithData(Easing::BEZIER);
    fadeCurve.addKey(0, 1, false);
    fadeCurve.selectItemWhenCreated = false;
    addChildControllableContainer(&fadeCurve);

}

LayoutFilter::~LayoutFilter()
{
}

FilterResult LayoutFilter::getFilteredResultForComponentInternal(Object* o, ObjectComponent* c)
{
    if (size->floatValue() == 0) return FilterResult();
   
    LayoutMode m = mode->getValueDataAsEnum<LayoutMode>();

    Vector3D<float> diffPos = o->stagePosition->getVector() - position->getVector();
    float diff = 0;

    switch (m)
    {
    case RADIUS: diff = diffPos.length(); break;
    case AXIS_X: diff = diffPos.x; break;
    case AXIS_Y: diff = diffPos.y; break;
    case AXIS_Z: diff = diffPos.z; break;
    }

    float p = diff / size->floatValue();
    float weight = fadeCurve.getValueAtPosition(p);

    if (weight <= 0) return FilterResult();
    return FilterResult({ o->globalID->intValue(), weight });
}
