/*
  ==============================================================================

    IDFilter.h
    Created: 3 Oct 2020 10:31:42am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class IDFilter :
    public Filter
{
public:
    IDFilter();
    ~IDFilter();

    ControllableContainer ids;
    
    bool isAffectingObject(Object* o) override;
    virtual FilterResult getFilteredResultForComponentInternal(Object* o, ObjectComponent * c) override;

    void controllableAdded(Controllable* c) override;

    var getJSONData(bool includeNonOverriden = false) override;
    void loadJSONDataInternal(var data) override;

    String getTypeString() const override { return "Filter by ID"; }
    static IDFilter* create(var params) { return new IDFilter(); }
};