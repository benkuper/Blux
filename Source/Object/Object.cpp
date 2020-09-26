/*
  ==============================================================================

    Object.cpp
    Created: 26 Sep 2020 10:02:32am
    Author:  bkupe

  ==============================================================================
*/

#include "Object.h"

Object::Object(var params) :
    BaseItem(params.getProperty("name","Object")),
    objectType(params.getProperty("type","Object").toString())
{
    itemDataType = "Object";
}

Object::~Object()
{
}
