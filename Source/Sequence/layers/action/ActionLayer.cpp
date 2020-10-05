/*
  ==============================================================================

    ActionLayer.cpp
    Created: 5 Oct 2020 4:02:52pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionLayer.h"
#include "../../BluxSequence.h"

ActionLayer::ActionLayer(Sequence* s, var params) :
    TriggerLayer(s, "Action", params)
{
}

ActionLayer::~ActionLayer()
{
}
