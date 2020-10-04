/*
  ==============================================================================

    IDFilter.cpp
    Created: 3 Oct 2020 10:31:42am
    Author:  bkupe

  ==============================================================================
*/

#include "IDFilter.h"
#include "Object/Object.h"

IDFilter::IDFilter() :
    Filter(getTypeString()),
    ids("IDs")
{
    //mode = addEnumParameter("Mode", "Filtering mode");
    //mode->addOption("Include", true)->addOption("Exclude", false);

    saveAndLoadRecursiveData = true;
    ids.userCanAddControllables = true;
    ids.userAddControllablesFilters.add(IntParameter::getTypeStringStatic());
    addChildControllableContainer(&ids);
}

IDFilter::~IDFilter()
{
}

int IDFilter::getFilteredIDForComponentInternal(Object* o, ObjectComponent* c)
{
    int oid = o->globalID->intValue();
    for (int i = 0; i < ids.controllables.size(); i++) if (oid == ((IntParameter*)ids.controllables[i])->intValue()) return i;
    return -1;
}
