/*
  ==============================================================================

    Effect.cpp
    Created: 30 Sep 2020 12:38:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "Effect.h"
#include "ui/EffectEditor.h"

Effect::Effect(const String& name, var params) :
    BaseItem(name)
{
    saveAndLoadRecursiveData = true;

    weight = addFloatParameter("Weight", "Weight of this effect",1,0,1);
    weight->hideInEditor = true;

    addChildControllableContainer(&filterManager);
    showInspectorOnSelect = false;
}

Effect::~Effect()
{
}

void Effect::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    var pValues = getProcessedComponentValuesInternal(o, c, values.clone());
    jassert(pValues.size() == values.size());
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray())
        {
            for (int j = 0; j < values[i].size(); j++)
            {
                values[i] = jmap<float>(weight->floatValue(), values[i][j], pValues[i][j]);
            }
        }
        else
        {
            values[i] = jmap<float>(weight->floatValue(), values[i], pValues[i]);
        }
    }
}

var Effect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values)
{
    return values;
}

void Effect::saveSceneData(var &sceneData)
{
    Array<WeakReference<Parameter>> params = getAllParameters();
    for (auto& p : params)
    {
        if (!p->hideInEditor && !p->isControllableFeedbackOnly) //BIG HACK to avoid listSize ViewUISize, etc.. should be in a proper list
        {
            sceneData.getDynamicObject()->setProperty(p->controlAddress, p->value);
        }
    } 
    sceneData.getDynamicObject()->setProperty(weight->getControlAddress(), weight->value);

    filterManager.saveSceneData(sceneData);
}

InspectableEditor* Effect::getEditor(bool isRoot)
{
    return new EffectEditor(this, isRoot);
}

//ThreadedEffect::ThreadedEffect(const String& name, var params) :
//    Effect(name, params),
//    Thread("Effect "+name)
//{
//    startThread();
//}
//
//ThreadedEffect::~ThreadedEffect()
//{
//    stopThread(1000);
//}
//
//void ThreadedEffect::onContainerParameterChangedInternal(Parameter* p)
//{
//    Effect::onContainerParameterChangedInternal(p);
//
//    if (p == enabled)
//    {
//        if (enabled->boolValue()) startThread();
//        else stopThread(1000);
//    }
//}
//
//void ThreadedEffect::run()
//{
//    while (!threadShouldExit())
//    {
//        long millisBefore = Time::getMillisecondCounter();
//        runInternal();
//        long millisAfter = Time::getMillisecondCounter();
//
//        long millisToSleep = jmax<long>(1, fps - (millisAfter - millisBefore));
//        sleep(millisToSleep);
//    }
//}
