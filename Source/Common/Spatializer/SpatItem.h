/*
  ==============================================================================

    SpatItem.h
    Created: 24 Dec 2023 10:04:48am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;


class SpatItem :
    public BaseItem
{
public:
    SpatItem(var params = var());
    ~SpatItem();

    Point<float> getObjectPosition(Object* o);

    DECLARE_TYPE("Spat")
};