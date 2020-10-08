/*
  ==============================================================================

    Action.cpp
    Created: 8 Oct 2020 3:33:09pm
    Author:  bkupe

  ==============================================================================
*/

#include "Action.h"

Action::Action(var params) :
    TimeTrigger(params.getProperty("type","Action").toString())
{
    flagY->hideInEditor = true;
    color->hideInEditor = true;

    typeString = params.getProperty("type", "Action").toString();
}

Action::~Action()
{

}
