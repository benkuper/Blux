/*
  ==============================================================================

    ShutterComponent.cpp
    Created: 26 Sep 2020 2:21:24pm
    Author:  bkupe

  ==============================================================================
*/
#include "Object/ObjectIncludes.h"

ShutterComponent::ShutterComponent(Object* o, var params) :
    ObjectComponent(o, getTypeString(), SHUTTER, params)
{
}

ShutterComponent::~ShutterComponent()
{
}
