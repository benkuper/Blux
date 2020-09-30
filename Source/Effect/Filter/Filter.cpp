/*
  ==============================================================================

    Filter.cpp
    Created: 26 Sep 2020 2:13:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter(const String& name) :
    BaseItem(name)
{
    itemDataType = "Filter";

    showInspectorOnSelect = false;
}

Filter::~Filter()
{
}
