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

    excludeFromScenes = addBoolParameter("Exclude From Scenes", "If checked, this effect won't be saved in scens", false);
    excludeFromScenes->hideInEditor = true;

    addChildControllableContainer(&filterManager);
    showInspectorOnSelect = false;
}

Effect::~Effect()
{
}

void Effect::processComponentValues(Object* o, ObjectComponent* c, var& values)
{
    FilterResult r = filterManager.getFilteredResultForComponent(o, c);
    if (r.id == -1) return;

    float targetWeight = r.weight * weight->floatValue();

    var pValues = getProcessedComponentValuesInternal(o, c, r.id, values.clone());
    jassert(pValues.size() == values.size() );
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].isArray())
        {
            jassert(pValues[i].isArray());
            for (int j = 0; j < values[i].size(); j++)
            {
                values[i][j] = jmap<float>(targetWeight, values[i][j], pValues[i][j]);
            }
        }
        else
        {
            values[i] = jmap<float>(targetWeight, values[i], pValues[i]);
        }
    }
}

var Effect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, int id, var values)
{
    return values;
}

void Effect::saveSceneData(var &sceneData)
{
    if (excludeFromScenes->boolValue()) return;

    Array<WeakReference<Parameter>> params = getAllParameters();
    for (auto& p : params)
    {
        if (p->type == Parameter::ENUM || p->type == Parameter::TARGET) continue;
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