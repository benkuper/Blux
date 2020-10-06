/*
  ==============================================================================

    IDFilter.h
    Created: 3 Oct 2020 10:31:42am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Filter.h"

class IDFilter :
    public Filter
{
public:
    IDFilter();
    ~IDFilter();

    ControllableContainer ids;
    
    virtual FilterResult getFilteredResultForComponentInternal(Object* o, ObjectComponent * c) override;

    String getTypeString() const override { return "Filter by ID"; }
    static IDFilter* create(var params) { return new IDFilter(); }
};