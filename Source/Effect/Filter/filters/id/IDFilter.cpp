/*
  ==============================================================================

    IDFilter.cpp
    Created: 3 Oct 2020 10:31:42am
    Author:  bkupe

  ==============================================================================
*/

IDFilter::IDFilter() :
    Filter(getTypeString()),
    ids("IDs")
{
    //mode = addEnumParameter("Mode", "Filtering mode");
    //mode->addOption("Include", true)->addOption("Exclude", false);

    ids.userCanAddControllables = true;
    ids.userAddControllablesFilters.add(IntParameter::getTypeStringStatic());
    addChildControllableContainer(&ids);
}

IDFilter::~IDFilter()
{
}

bool IDFilter::isAffectingObject(Object* o)
{
    int oid = o->globalID->intValue();
    for (int i = 0; i < ids.controllables.size(); i++) if (oid == ((IntParameter*)ids.controllables[i])->intValue()) return true;
    return false;
}

FilterResult IDFilter::getFilteredResultForComponentInternal(Object* o, ObjectComponent* c)
{
    int oid = o->globalID->intValue();
    for (int i = 0; i < ids.controllables.size(); i++) if (oid == ((IntParameter*)ids.controllables[i])->intValue()) return FilterResult({ i, 1 });
    return FilterResult();
}

void IDFilter::controllableAdded(Controllable* c)
{
    if (c->parentContainer == &ids && !isCurrentlyLoadingData)
    {
        c->setNiceName("ID");
        ((IntParameter*)c)->setRange(0, INT32_MAX);
    }
}

var IDFilter::getJSONData()
{
    var data = Filter::getJSONData();
    var idData;
    for (auto& c : ids.controllables)
    {
        idData.append(((Parameter*)c)->intValue());
    }
    data.getDynamicObject()->setProperty("ids", idData);
    return data;
}

void IDFilter::loadJSONDataInternal(var data)
{
    Filter::loadJSONDataInternal(data);
    var idData = data.getProperty("ids", var());
    for (int i = 0; i < idData.size(); i++)
    {
        IntParameter* ip = ids.addIntParameter("ID", "", 0, 0);
        ip->setValue((int)idData[i]);
    }
}
