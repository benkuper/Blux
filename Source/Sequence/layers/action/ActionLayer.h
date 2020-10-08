/*
  ==============================================================================

    ActionLayer.h
    Created: 5 Oct 2020 4:02:52pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Action.h"

class ActionLayer :
    public TriggerLayer
{
public:
    ActionLayer(Sequence* s, var params);
    ~ActionLayer();

    String getTypeString() const override { return "Actions"; }
    static ActionLayer* create(Sequence* s, var params) { return new ActionLayer(s, params); }
};

class ActionManager :
    public TimeTriggerManager
{
public:
    ActionManager(ActionLayer* layer, Sequence* sequence);
    ~ActionManager();

};


class ActionFactory :
    public Factory<TimeTrigger>
{
public:
    juce_DeclareSingleton(ActionFactory, true);
    ActionFactory();
};
