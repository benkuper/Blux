/*
  ==============================================================================

    FilterManager.cpp
    Created: 26 Sep 2020 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "FilterManager.h"
#include "filters/id/IDFilter.h"
#include "filters/layout/LayoutFilter.h"
#include "filters/group/GroupFilter.h"

#include "Object/Object.h"
#include "ui/FilterManagerEditor.h"

FilterManager::FilterManager() :
    BaseManager("Filters")
{
    itemDataType = "Filter";

    managerFactory = &factory;
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Filter by ID", &IDFilter::create));
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Filter by Group", &GroupFilter::create));
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Layout Filter", &LayoutFilter::create));
}

FilterManager::~FilterManager()
{
}

var FilterManager::getSceneData()
{
    var data(new DynamicObject());
    for (auto& i : items) data.getDynamicObject()->setProperty(i->shortName, i->getSceneData());
    return data;
}

void FilterManager::updateSceneData(var& sceneData)
{
}

void FilterManager::lerpFromSceneData(var startData, var endData, float weight)
{
    for (auto& i : items) i->lerpFromSceneData(startData.getProperty(i->shortName, var()), endData.getProperty(i->shortName, var()), weight);
}

bool FilterManager::isAffectingObject(Object* o)
{
    bool hasSelectedComponents = false;
    for (auto& c : o->componentManager.items)
    {
        if (componentSelector.selectedComponents[c->componentType])
        {
            hasSelectedComponents = true;
            break;
        }
    }
    
    if (!hasSelectedComponents) return false;

    if (items.size() == 0) return true;

    for (auto& f : items)
    {
        if (f->isAffectingObject(o)) return true;
    }

    return false;
}

FilterResult FilterManager::getFilteredResultForComponent(Object* o, ObjectComponent* c)
{
    if (c != nullptr && !componentSelector.selectedComponents[c->componentType]) return FilterResult();

    bool hasFilteredAtLeastOnce = false;
    for (auto& f : items)
    {
        if (!f->enabled->boolValue()) continue;
        hasFilteredAtLeastOnce = true;

        FilterResult r = f->getFilteredResultForComponent(o, c);
        if (r.id >= 0) return r;
    }

    return hasFilteredAtLeastOnce ? FilterResult() : FilterResult({ o->globalID->intValue(), 1 });
}

InspectableEditor* FilterManager::getEditor(bool isRoot)
{
    return new FilterManagerEditor(this, isRoot);
}

var FilterManager::getJSONData()
{
    var data = BaseManager::getJSONData();
    data.getDynamicObject()->setProperty("selectedComponents", componentSelector.getJSONData());
    return data;
}

void FilterManager::loadJSONDataManagerInternal(var data)
{
    BaseManager::loadJSONDataManagerInternal(data);
    componentSelector.loadJSONData(data.getProperty("selectedComponents", var()));
}
