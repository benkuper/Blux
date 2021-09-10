/*
  ==============================================================================

    Object2DView.cpp
    Created: 6 Oct 2020 10:03:37pm
    Author:  bkupe

  ==============================================================================
*/

Object2DView::Object2DView(Object* object) :
    ObjectGridUI(object)
{
    transparentBG = true;
    autoHideWhenDragging = false;
    drawEmptyDragIcon = true;

    float s = StageLayoutManager::getInstance()->iconSize->floatValue();
    setSize(s, s);
}

Object2DView::~Object2DView()
{
}
