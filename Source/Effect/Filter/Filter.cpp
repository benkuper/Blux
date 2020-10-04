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

    useLocalID = addBoolParameter("Use Local ID", "If enabled, this will use the generated ID from this filter instead of the object's global ID", false);
    showInspectorOnSelect = false;

}

Filter::~Filter()
{
}

int Filter::getFilteredIDForComponent(Object* o, ObjectComponent* c)
{
    if (!enabled->boolValue()) return -1;
    
    int localID = getFilteredIDForComponentInternal(o, c);
    if (localID == -1) return -1;

    return useLocalID->boolValue() ? localID : o->globalID->intValue();
}

int Filter::getFilteredIDForComponentInternal(Object* o, ObjectComponent* c)
{
    return o->globalID->intValue();
}

void Filter::saveSceneData(var& sceneData)
{
    Array<WeakReference<Parameter>> params = getAllParameters();
    for (auto& p : params)
    {
        if (!p->hideInEditor && !p->isControllableFeedbackOnly) //BIG HACK to avoid listSize ViewUISize, etc.. should be in a proper list
        {
            sceneData.getDynamicObject()->setProperty(p->controlAddress, p->value);
        }
    }
}
