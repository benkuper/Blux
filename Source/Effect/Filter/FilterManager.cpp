/*
  ==============================================================================

    FilterManager.cpp
    Created: 26 Sep 2020 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "FilterManager.h"
#include "filters/component/ComponentFilter.h"
#include "Object/Object.h"

FilterManager::FilterManager(bool hasComponentFilter, bool hasObjectFilter) :
    BaseManager("Filters")
{
    itemDataType = "Filter";

    managerFactory = &factory;
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Component", &ComponentFilter::create));
}

FilterManager::~FilterManager()
{
}

int FilterManager::getFilteredIDForObject(Object* o)
{
    for (auto& e : items)
    {
        int id = e->getFilteredIDForObject(o);
        if (id >= 0) return id;
    }

    return o->globalID->intValue();
}
