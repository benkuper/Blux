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
#include "Effect/Filter/filters/layout/ui/LayoutFilterStageView.h"

class StageLayout2DView :
    public BaseManagerShapeShifterViewUI<ObjectManager, Object, Object2DView>,
    public ContainerAsyncListener
{
public:
    StageLayout2DView(const String &name);
    ~StageLayout2DView();

    std::unique_ptr<FloatSliderUI> iconSizeUI;
    std::unique_ptr<BoolToggleUI> showFiltersUI;

    LayoutFilterStageView filterStageView;

    void addItemUIInternal(Object2DView* ui) override;

    void resized() override;

    void newMessage(const ContainerAsyncEvent& e) override;

    static StageLayout2DView* create(const String& name) { return new StageLayout2DView(name); }
};