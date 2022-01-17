/*
  ==============================================================================

    FreezeEffect.cpp
    Created: 11 Sep 2021 5:16:02pm
    Author:  bkupe

  ==============================================================================
*/

FreezeEffect::FreezeEffect(const String& name, var params) :
    Effect(name, params)
{
    filterManager->componentSelector.allowedComponents.removeAllInstancesOf(ComponentType::COLOR);
    filterManager->componentSelector.selectedComponents.set(ComponentType::INTENSITY, true);
    filterManager->componentSelector.selectedComponents.set(ComponentType::PANTILT, true);
    filterManager->componentSelector.selectedComponents.set(ComponentType::SERVO, true);
    filterManager->componentSelector.selectedComponents.set(ComponentType::STEPPER, true);

    freezeMode = effectParams.addEnumParameter("Mode", "Keep only higher values or lower values");
    freezeMode->addOption("Hold",HOLD)->addOption("Increase Only", MAX)->addOption("Decrease Only", MIN);

    reset = effectParams.addTrigger("Reset", "Reset the freeze");
}

FreezeEffect::~FreezeEffect()
{
}

void FreezeEffect::onContainerTriggerTriggered(Trigger* t)
{
    Effect::onContainerTriggerTriggered(t);
    if (t == reset) prevValues.clear();
}

void FreezeEffect::onContainerParameterChangedInternal(Parameter* p)
{
    Effect::onContainerParameterChangedInternal(p);
    if (p == enabled)
    {
        if (enabled->boolValue()) prevValues.clear();
    }
    else if (p == weight)
    {
        if (weight->floatValue() == 0) prevValues.clear();
    }
}

var FreezeEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{

    if (!prevValues.contains(c)) prevValues.set(c, values.clone());
    var newValues = getProcessedComponentValuesFreezeInternal(o, c, values, id, time);
    if(freezeMode->getValueDataAsEnum<FreezeMode>() != HOLD) prevValues.set(c, newValues.clone());

    return newValues;
}


FreezeFloatEffect::FreezeFloatEffect(var params) :
    FreezeEffect(getTypeString(), params)
{
    
}

FreezeFloatEffect::~FreezeFloatEffect()
{
}

var FreezeFloatEffect::getProcessedComponentValuesFreezeInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{
    FreezeMode m = freezeMode->getValueDataAsEnum<FreezeMode>();

    var curPrevValues = prevValues[c];
    if (m == HOLD) return curPrevValues;

    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray())
        {
            for (int j = 0; j < values.size(); j++)
            {
                values[i][j] = m == MAX ? jmax((float)curPrevValues[i][j], (float)values[i][j]) : jmin((float)curPrevValues[i][j], (float)values[i][j]);
            }
        }
        else
        {
            values[i] = m == MAX ? jmax((float)curPrevValues[i], (float)values[i]) : jmin((float)curPrevValues[i], (float)values[i]);
        }
    }

    return values;
}