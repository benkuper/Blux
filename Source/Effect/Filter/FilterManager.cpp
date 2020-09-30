/*
  ==============================================================================

    FilterManager.cpp
    Created: 26 Sep 2020 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "FilterManager.h"
#include "filters/component/ComponentFilter.h"

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
