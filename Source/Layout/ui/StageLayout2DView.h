/*
  ==============================================================================

    StageLayout2DView.h
    Created: 6 Oct 2020 9:49:35pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Object/ObjectManager.h"
#include "Object2DView.h"

class StageLayout2DView :
    public BaseManagerShapeShifterViewUI<ObjectManager, Object, Object2DView>
{
public:
    StageLayout2DView(const String &name);
    ~StageLayout2DView();

    std::unique_ptr<FloatSliderUI> iconSizeUI;

    void resizedInternalHeader(Rectangle<int>& r) override;

    static StageLayout2DView* create(const String& name) { return new StageLayout2DView(name); }
};