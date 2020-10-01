/*
  ==============================================================================

    Filter.cpp
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "Filter.h"
#include "Object/Object.h"

Filter::Filter(const String& name) :
    BaseItem(name)
{
    itemDataType = "Filter";

    showInspectorOnSelect = false;
}

Filter::~Filter()
{
}

int Filter::getFilteredIDForObject(Object* o)
{
    return -1;
}
