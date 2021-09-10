/*
  ==============================================================================

    Object2DView.h
    Created: 6 Oct 2020 10:03:37pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object2DView :
    public ObjectGridUI
{
public:
	Object2DView(Object* object);
	~Object2DView();
};