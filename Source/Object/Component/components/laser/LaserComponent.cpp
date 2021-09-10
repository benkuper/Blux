/*
  ==============================================================================

    LaserComponent.cpp
    Created: 26 Sep 2020 2:20:24pm
    Author:  bkupe

  ==============================================================================
*/

LaserComponent::LaserComponent(Object* o, var params) :
    ObjectComponent(o, getTypeString(), LASER, params)
{
}

LaserComponent::~LaserComponent()
{
}
