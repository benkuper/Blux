/*
  ==============================================================================

    Object2DView.cpp
    Created: 6 Oct 2020 10:03:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "Object2DView.h"

Object2DView::Object2DView(Object* object) :
    ObjectGridUI(object)
{
    transparentBG = true;
    autoHideWhenDragging = false;
    drawEmptyDragIcon = true;
}

Object2DView::~Object2DView()
{
}
