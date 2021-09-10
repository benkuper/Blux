/*
  ==============================================================================

    EffectBlockManagerUI.cpp
    Created: 10 Oct 2020 10:49:03am
    Author:  bkupe

  ==============================================================================
*/

EffectBlockManagerUI::EffectBlockManagerUI(EffectLayerTimeline * timeline) :
    LayerBlockManagerUI(timeline, &timeline->effectLayer->blockManager),
    effectTimeline(timeline)
{
    addExistingItems();
}

EffectBlockManagerUI::~EffectBlockManagerUI()
{
}


LayerBlockUI* EffectBlockManagerUI::createUIForItem(LayerBlock* b)
{
    return new EffectBlockUI((EffectBlock*)b);
}

void EffectBlockManagerUI::showMenuAndAddItem(bool fromAddButton, Point<int> mouseDownPos)
{
    if (LayerBlock* b = effectTimeline->effectLayer->blockManager.managerFactory->showCreateMenu())
    {
        manager->addBlockAt(b, timeline->getTimeForX(mouseDownPos.x));
    }
}


void EffectBlockManagerUI::mouseDoubleClick(const MouseEvent& e)
{
    //nothing, do not create on double click
}
