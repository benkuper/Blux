/*
  ==============================================================================

    GroupFilter.h
    Created: 7 Oct 2020 4:11:09pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class GroupFilter :
    public Filter
{
public:
    GroupFilter();
    ~GroupFilter();

    ControllableContainer groups;
    BoolParameter* invertOrder;

    void controllableAdded(Controllable*) override;

    bool isAffectingObject(Object* o) override;
    virtual FilterResult getFilteredResultForComponentInternal(Object* o, ObjectComponent* c) override;

    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;

    String getTypeString() const override { return "Filter by Group"; }
    static GroupFilter* create(var params) { return new GroupFilter(); }
};