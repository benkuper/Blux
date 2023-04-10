/*
  ==============================================================================

    FilterManager.cpp
    Created: 26 Sep 2020 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

FilterManager::FilterManager() :
    BaseManager("Filters")
{
    itemDataType = "Filter";

    managerFactory = &factory;
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Filter by ID", &IDFilter::create));
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Filter by Group", &GroupFilter::create));
    factory.defs.add(Factory<Filter>::Definition::createDef("", "Layout Filter", &LayoutFilter::create));

    weightOperator = addEnumParameter("Operator", "Decides how filters are working together.");
    weightOperator->addOption("Max", MAX)->addOption("Min", MIN)->addOption("Multiply", MULTIPLY);
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
    //bool hasSelectedComponents = false;
    //for (auto& c : o->componentManager->items)
    //{
    //    if (componentSelector.selectedComponents[c->componentType])
    //    {
    //        hasSelectedComponents = true;
    //        break;
    //    }
    //}
    
    //if (!hasSelectedComponents) return false;

    if (items.size() == 0) return true;

    WeightOperator wo = weightOperator->getValueDataAsEnum<WeightOperator>();

    for (auto& f : items)
    {
        if (f->isAffectingObject(o))
        {
            if (wo == MAX || wo == MULTIPLY) return true;
        }
        else
        {
            if (wo == MIN) return false;
        }
    }

    return false;
}

FilterResult FilterManager::getFilteredResultForComponent(Object* o, ObjectComponent* c)
{
    //if (c != nullptr && !componentSelector.selectedComponents[c->componentType]) return FilterResult();

    bool hasFilteredAtLeastOnce = false;

    WeightOperator wo = weightOperator->getValueDataAsEnum<WeightOperator>();
    FilterResult result({ o->globalID->intValue(), 1.f });

    for (auto& f : items)
    {
        if (!f->enabled->boolValue()) continue;
        if (!hasFilteredAtLeastOnce)
        {
            hasFilteredAtLeastOnce = true;
            result = FilterResult(); //force no ID if filtered but not affected
            if (wo == MAX) result.weight = 0; //initialize weight
        }

        FilterResult r = f->getFilteredResultForComponent(o, c);
        //if (r.id >= 0) return r;
        if (r.id >= 0)
        {
            result.id = r.id;
            if (wo == MIN) result.weight = jmin(result.weight, r.weight);
            else if (wo == MAX) result.weight = jmax(result.weight, r.weight);
            else if (wo == MULTIPLY) result.weight *= r.weight;
        }
        else
        {
            if (wo == MIN || wo == MULTIPLY) return FilterResult();
        }
    }

    return result;
}

InspectableEditor* FilterManager::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
    return new FilterManagerEditor(this, isRoot);
}

var FilterManager::getJSONData()
{
    var data = BaseManager::getJSONData();
    //data.getDynamicObject()->setProperty("selectedComponents", componentSelector.getJSONData());
    return data;
}

void FilterManager::loadJSONDataManagerInternal(var data)
{
    BaseManager::loadJSONDataManagerInternal(data);
    //componentSelector.loadJSONData(data.getProperty("selectedComponents", var()));
}
