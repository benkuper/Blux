/*
  ==============================================================================

    ObjectUI.h
    Created: 26 Sep 2020 7:51:11pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ObjectUI :
    public ItemUI<Object>
{
public:
    ObjectUI(Object* item);
    virtual ~ObjectUI();
};