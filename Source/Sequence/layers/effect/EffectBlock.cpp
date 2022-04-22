/*
  ==============================================================================

    EffectBlock.cpp
    Created: 10 Oct 2020 10:46:50am
    Author:  bkupe

  ==============================================================================
*/

EffectBlock::EffectBlock(var params) :
    LayerBlock(params.getProperty("effectType",var()).toString()),
    effectType(params.getProperty("effectType", var()).toString()),
    settingLengthFromMethod(false),
    effectBlockNotifier(5)
{

    saveAndLoadRecursiveData = true;

    fadeIn = addFloatParameter("Fade In", "Fade in time", 0, 0, getTotalLength(), false);
    fadeIn->defaultUI = FloatParameter::TIME;
    fadeIn->canBeDisabledByUser = true;
    fadeOut = addFloatParameter("Fade Out", "Fade out time", 0, 0, getTotalLength(), false);
    fadeOut->defaultUI = FloatParameter::TIME;
    fadeOut->canBeDisabledByUser = true;

    effect.reset(EffectFactory::getInstance()->create(effectType));
    if (effect != nullptr)
    {
        effect->editorCanBeCollapsed = false;
        effect->canBeDisabled = false;
        //effect->enabled->setValue(true); //this way, timeEffect won't start their timers.

        if (TimedEffect* te = dynamic_cast<TimedEffect*>(effect.get()))
        {
            te->forceManualTime = true;
            //te->speed->hideInEditor = true; //hide speed since it cannot be used alongside time
        }

        effect->addEffectListener(this);
        addChildControllableContainer(effect.get());
    }
}

EffectBlock::~EffectBlock()
{

}

void EffectBlock::processComponentValues(Object* o, ObjectComponent* c, var& values, float weightMultiplier, int id, float absoluteTime)
{
    if (effect == nullptr) return;

	float factor = 1;
	double relTimeTotal = absoluteTime - time->floatValue();
	if (fadeIn->floatValue() > 0) factor *= jmin<double>(relTimeTotal / fadeIn->floatValue(), 1.f);
	if (fadeOut->floatValue() > 0) factor *= jmin<double>((getTotalLength() - relTimeTotal) / fadeOut->floatValue(), 1.f);
	factor = jmax(factor, 0.f);

    float targetWeight = factor * weightMultiplier;

	double relTimeLooped = getRelativeTime(absoluteTime, true);
    effect->processComponentValues(o, c, values, targetWeight, id, relTimeLooped);
    if (c->componentType == ComponentType::INTENSITY && values.size() > 0) o->effectIntensityOutMap.set(effect.get(), values[0].clone());
}

void EffectBlock::onContainerParameterChangedInternal(Parameter* p)
{
    LayerBlock::onContainerParameterChangedInternal(p);

    if (p == coreLength || p == loopLength)
	{
		fadeIn->setRange(0, getTotalLength());
		fadeOut->setRange(0, getTotalLength());

        if (p == coreLength && !settingLengthFromMethod) //force refresh automation
        {
            setCoreLength(coreLength->floatValue());
        }
    }
    else if (p == fadeIn || p == fadeOut)
    {
        effectBlockListeners.call(&EffectBlockListener::effectBlockFadesChanged, this);
        effectBlockNotifier.addMessage(new EffectBlockEvent(EffectBlockEvent::FADES_CHANGED, this));
    }
}

void EffectBlock::controllableStateChanged(Controllable* c)
{
    LayerBlock::controllableStateChanged(c);
    if (c == fadeIn || c == fadeOut)
    {
        effectBlockListeners.call(&EffectBlockListener::effectBlockFadesChanged, this);
        effectBlockNotifier.addMessage(new EffectBlockEvent(EffectBlockEvent::FADES_CHANGED, this));
    }
}

void EffectBlock::setCoreLength(float value, bool stretch, bool stickToCoreEnd)
{
    settingLengthFromMethod = true;
    LayerBlock::setCoreLength(value, stretch, stickToCoreEnd);

    Array<WeakReference<Parameter>> params = effect->effectParams.getAllParameters();
    for (auto& pa : params)
    {
        if (pa->automation == nullptr) continue;
        pa->automation->setAllowKeysOutside(true);
        pa->automation->setLength(coreLength->floatValue(), stretch, stickToCoreEnd);
    }
    settingLengthFromMethod = false;
}

void EffectBlock::effectParamControlModeChanged(Parameter* p)
{
    if (p->controlMode == Parameter::AUTOMATION)
    {
  
        p->automation->setManualMode(true);
        p->automation->setAllowKeysOutside(true);
        p->automation->setLength(coreLength->floatValue(), true);
    }
}
