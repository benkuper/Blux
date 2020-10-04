/*
  ==============================================================================

    FilterManager.cpp
    Created: 26 Sep 2020 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "FilterManager.h"
#include "filters/id/IDFilter.h"
#include "Object/Object.h"
#include "ui/FilterManagerEditor.h"

FilterManager::FilterManager() :
    BaseManager("Filters")
{
    itemDataType = "Filter";

    managerFactory = &factory;
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Filter by ID", &IDFilter::create));
}

FilterManager::~FilterManager()
{
}

void FilterManager::saveSceneData(var& sceneData)
{
    for (auto& i : items) i->saveSceneData(sceneData);
}

int FilterManager::getFilteredIDForComponent(Object* o, ObjectComponent* c)
{
    if (!componentSelector.selectedComponents[c->componentType]) return -1;
   
    bool hasFilteredAtLeastOnce = false;

    for (auto& e : items)
    {
        if (!e->enabled->boolValue()) continue;
        hasFilteredAtLeastOnce = true;

        int id = e->getFilteredIDForComponent(o, c);
        if (id >= 0) return id;
    }

    return hasFilteredAtLeastOnce ? -1 : o->globalID->intValue();
}

InspectableEditor* FilterManager::getEditor(bool isRoot)
{
    return new FilterManagerEditor(this, isRoot);
}
