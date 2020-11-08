/*
  ==============================================================================

    ObjectEditor.h
    Created: 8 Nov 2020 11:17:00pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Object.h"

class ColorSourceViz;

class ObjectEditor :
    public BaseItemEditor
{
public:
    ObjectEditor(Object* o, bool isRoot);
    ~ObjectEditor();

    Object* object;

    //std::unique_ptr<ColorSourceViz> viz;

    //void resizedInternalContent(Rectangle<int> &r) override;
};