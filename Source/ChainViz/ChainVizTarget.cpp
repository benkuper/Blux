/*
  ==============================================================================

    ChainVizTarget.cpp
    Created: 31 Oct 2020 10:49:00am
    Author:  bkupe

  ==============================================================================
*/

#include "ChainVizTarget.h"
#include "ChainViz.h"
#include "Object/Object.h"

ChainVizTarget::~ChainVizTarget() 
{
}


ChainVizComponent* ChainVizTarget::createVizComponent(Object * o, ChainVizType type) {
    return new ChainVizComponent(this, o, type);
}
