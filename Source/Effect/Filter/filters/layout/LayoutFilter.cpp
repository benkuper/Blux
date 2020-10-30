/*
  ==============================================================================

    LayoutFilter.cpp
    Created: 6 Oct 2020 11:32:40pm
    Author:  bkupe

  ==============================================================================
*/

#include "LayoutFilter.h"
#include "Object/Object.h"
#include "Layout/StageLayoutManager.h"

Array<LayoutFilter*> LayoutFilter::instances;
ChangeBroadcaster LayoutFilter::broadcaster;

LayoutFilter::LayoutFilter() :
    Filter(getTypeString()),
    fadeCurve("Fade Curve")
{
    layout = addTargetParameter("Layout", "The layout to use. Leave blank or disable to use current layout", StageLayoutManager::getInstance(), false);
    layout->maxDefaultSearchLevel = 0;
    layout->targetType = TargetParameter::CONTAINER;
    layout->canBeDisabledByUser = true;

    enableInView = addBoolParameter("Enable in view", "If checked, this effect will be visible in the stage layout view", true);
    colorInView = addColorParameter("Color in view", "Color in the view", Colours::red);
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

    instances.addIfNotAlreadyThere(this);
    broadcaster.sendChangeMessage();
}

LayoutFilter::~LayoutFilter()
{
}

void LayoutFilter::clearItem()
{
    instances.removeAllInstancesOf(this);
    broadcaster.sendChangeMessage();
}

bool LayoutFilter::isAffectingObject(Object* o)
{
    return true;
}

FilterResult LayoutFilter::getFilteredResultForComponentInternal(Object* o, ObjectComponent* c)
{
    if (size->floatValue() == 0) return FilterResult();
   
    LayoutMode m = mode->getValueDataAsEnum<LayoutMode>();

    Vector3D<float> pos = (layout->enabled && layout->targetContainer != nullptr) ? ((StageLayout *)layout->targetContainer.get())->getObjectPosition(o) : o->stagePosition->getVector();
    Vector3D<float> diffPos = pos - position->getVector();
    float diff = 0;

    switch (m)
    {
    case RADIUS: diff = diffPos.length(); break;
    case AXIS_X: diff = diffPos.x + size->floatValue() / 2; break;
    case AXIS_Y: diff = diffPos.y + size->floatValue() / 2; break;
    case AXIS_Z: diff = diffPos.z + size->floatValue() / 2; break;
    }

    float p = diff / size->floatValue();
    float weight = fadeCurve.getValueAtPosition(p);

    if (weight <= 0) return FilterResult();
    return FilterResult({ o->globalID->intValue(), weight });
}
