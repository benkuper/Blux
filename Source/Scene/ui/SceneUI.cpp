/*
  ==============================================================================

    SceneUI.cpp
    Created: 26 Sep 2020 7:49:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "SceneUI.h"
#include "../SceneManager.h"
#include "Object/ui/ObjectManagerGridUI.h"
#include "Layout/ui/StageLayout2DView.h"

SceneUI::SceneUI(Scene* scene) :
    BaseItemUI(scene)
{
    loadUI.reset(item->loadTrigger->createButtonUI());
    addAndMakeVisible(loadUI.get());
    loadProgressUI.reset(item->loadProgress->createSlider());
    addAndMakeVisible(loadProgressUI.get());
    loadProgressUI->setVisible(item->defaultLoadTime->floatValue() > 0);
    loadProgressUI->showLabel = false;
    loadProgressUI->showValue = false;

    acceptedDropTypes.add("Container");
}

SceneUI::~SceneUI()
{
}

void SceneUI::resizedInternalHeader(Rectangle<int>& r)
{
    loadUI->setBounds(r.removeFromRight(60).reduced(2));
    r.removeFromRight(2);
    loadProgressUI->setBounds(r.removeFromRight(100).reduced(2,4));
}

void SceneUI::mouseEnter(const MouseEvent& e)
{
    BaseItemUI::mouseEnter(e);
    if (SceneManager::getInstance()->autoPreview->boolValue()) showPreview(true);

}

void SceneUI::mouseExit(const MouseEvent& e)
{
    BaseItemUI::mouseExit(e);
    showPreview(false);
}

void SceneUI::mouseDown(const MouseEvent& e)
{
    BaseItemUI::mouseDown(e);
    if (e.mods.isLeftButtonDown())
    {
        if (e.mods.isAltDown())
        {
            if (e.mods.isCommandDown()) item->saveScene();
            else item->loadScene(0);
        }
    }
    else if (e.mods.isRightButtonDown())
    {
        PopupMenu m;
        m.addItem(1, "Save scene");
        m.addItem(2, "Load scene");
        int result = m.show();
        switch (result)
        {
        case 1:
            item->saveScene();
            break;

        case 2:
            item->loadScene();
            break;
        }
    }
}

void SceneUI::itemDropped(const DragAndDropTarget::SourceDetails& details)
{
    String dataType = details.description.getProperty("dataType", "");
    if (dataType == "Container")
    {
        if (BaseItemEditor * bi = dynamic_cast<BaseItemEditor *>(details.sourceComponent.get()))
        {
            if (Effect* e = dynamic_cast<Effect*>(bi->item))
            {
                item->effectManager.addItemFromData(e->getJSONData());
            }
        }
        else if (GenericManagerEditor<Effect>* gi = dynamic_cast<GenericManagerEditor<Effect>*>(details.sourceComponent.get()))
        {
            item->effectManager.loadJSONData(gi->manager->getJSONData());
        }
    }
    else
    {
        NLOGWARNING(item->niceName, "Drop not supported for type : " << dataType);
    }
}

void SceneUI::showPreview(bool doShow)
{
    var d = doShow ? item->sceneData.getProperty(ObjectManager::getInstance()->shortName, var()) : var();
    if (ObjectManagerGridUI* grid = ShapeShifterManager::getInstance()->getContentForType<ObjectManagerGridUI>()) grid->setPreviewData(d);
    if (StageLayout2DView* view = ShapeShifterManager::getInstance()->getContentForType<StageLayout2DView>()) view->setPreviewData(d);
}

void SceneUI::controllableFeedbackUpdateInternal(Controllable* c)
{
    BaseItemUI::controllableFeedbackUpdateInternal(c);
    if (c == item->defaultLoadTime)
    {
        loadProgressUI->setVisible(item->defaultLoadTime->floatValue() > 0);
    }
    else if (c == item->loadProgress || c == item->isCurrent)
    {
        bgColor = item->isCurrent->boolValue() ? BLUE_COLOR : (item->loadProgress->floatValue() > 0 ? HIGHLIGHT_COLOR : BG_COLOR.brighter(.1f));
        repaint();
    }
}
