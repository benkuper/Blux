/*
  ==============================================================================

    Filter.cpp
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "Filter.h"
#include "Object/Object.h"

#include "ui/FilterEditor.h"

Filter::Filter(const String& name) :
    BaseItem(name)
{
    itemDataType = "Filter";
    saveAndLoadRecursiveData = true;
    useLocalID = addBoolParameter("Use Local ID", "If enabled, this will use the generated ID from this filter instead of the object's global ID", false);
    showInspectorOnSelect = false;

}

Filter::~Filter()
{
}

FilterResult Filter::getFilteredResultForComponent(Object* o, ObjectComponent* c)
{
    if (!enabled->boolValue()) return FilterResult();
    
    FilterResult r = getFilteredResultForComponentInternal(o, c);
    if (r.id == -1) return r;

    return FilterResult({ useLocalID->boolValue() ? r.id : o->globalID->intValue(), r.weight });
}

FilterResult Filter::getFilteredResultForComponentInternal(Object* o, ObjectComponent* c)
{
    return FilterResult({ o->globalID->intValue(), 1 });
}

void Filter::saveSceneData(var& sceneData)
{
    Array<WeakReference<Parameter>> params = getAllParameters();
    for (auto& p : params)
    {
        if (p->type == Parameter::ENUM || p->type == Parameter::TARGET) continue;
        if (!p->hideInEditor && !p->isControllableFeedbackOnly) //BIG HACK to avoid listSize ViewUISize, etc.. should be in a proper list
        {
            sceneData.getDynamicObject()->setProperty(p->controlAddress, p->value);
        }
    }
}
