/*
  ==============================================================================

    AutomationEffect.cpp
    Created: 4 Oct 2020 11:37:20pm
    Author:  bkupe

  ==============================================================================
*/

#include "AutomationEffect.h"

AutomationEffect::AutomationEffect(var params) :
    Effect(getTypeString(), params),
    automation("Curve"),
    curTime(0)
{
    length = addFloatParameter("Length", "Length of this automation", 2, .1f);
    length->defaultUI = FloatParameter::TIME;

    speed = addFloatParameter("Speed", "The speed at which play this", 1);
    range = addPoint2DParameter("Range", "Range of this automation");

    var val;
    val.append(0);
    val.append(1);
    range->defaultValue = val;
    range->resetValue();

    offsetByID = addFloatParameter("Time Offset By ID", "Time Offset by object ID", 0);
    offsetByValue = addFloatParameter("Time Offset By Value", "Time Offset by parameter inside a component", 0);

    automation.isSelectable = false;
    automation.length->setValue(1);
    automation.addKey(0,0 , false);
    automation.items[0]->easingType->setValueWithData(Easing::BEZIER);
    automation.addKey(.5f, 1, false);
    automation.items[0]->easingType->setValueWithData(Easing::BEZIER);
    automation.addKey(1, 0, false);
    automation.selectItemWhenCreated = false;
    automation.rangeRemapMode->setValueWithData(Automation::RangeRemapMode::PROPORTIONAL);

    addChildControllableContainer(&automation);
    timeAtLastUpdate = Time::getMillisecondCounter() / 1000.0;

    filterManager.componentSelector.selectedComponents.set(ObjectComponent::INTENSITY, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::PANTILT, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::SERVO, true);
    filterManager.componentSelector.selectedComponents.set(ObjectComponent::STEPPER, true);

    startTimer(20);
}

AutomationEffect::~AutomationEffect()
{
}

var AutomationEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values)
{
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray()) continue;
        
        float time = curTime + offsetByID->floatValue() * id + offsetByValue->floatValue() * i;
        float relTime = fmodf(time, length->floatValue());
        if (relTime < 0) relTime += length->floatValue();
        values[i] = automation.getValueAtPosition(relTime / length->floatValue());
    }

    return values;
}

void AutomationEffect::onContainerParameterChangedInternal(Parameter* p)
{
    Effect::onContainerParameterChangedInternal(p);
    if (p == enabled)
    {
        if (enabled->boolValue())
        {
            timeAtLastUpdate = Time::getMillisecondCounterHiRes() / 1000.0;
            curTime = 0;
            startTimer(20);
        }
        else
        {
            //stopTimer();
        }
    }
    else if (p == range)
    {
        automation.valueRange->setPoint(range->getPoint());
    }

}

void AutomationEffect::timerCallback()
{
    double newTime = Time::getMillisecondCounter() / 1000.0;
    curTime += (newTime - timeAtLastUpdate) * speed->floatValue();
    timeAtLastUpdate = newTime;
}
