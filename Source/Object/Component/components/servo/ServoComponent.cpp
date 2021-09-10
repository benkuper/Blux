/*
  ==============================================================================

    ServoComponent.cpp
    Created: 26 Sep 2020 2:21:12pm
    Author:  bkupe

  ==============================================================================
*/

ServoComponent::ServoComponent(Object* o, var params) :
    ObjectComponent(o, getTypeString(), SERVO,  params)
{
}

ServoComponent::~ServoComponent()
{
}
