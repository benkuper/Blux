/*
  ==============================================================================

	SceneUI.cpp
	Created: 26 Sep 2020 7:49:24pm
	Author:  bkupe

  ==============================================================================
*/


#include "Scene/SceneIncludes.h"

Image SceneUI::fxImage;
Image SceneUI::seqImage;



SceneUI::SceneUI(Scene* scene) :
	BaseItemUI(scene)
{

	if(fxImage.isNull()) fxImage = ImageCache::getFromMemory(BinaryData::fx_png, BinaryData::fx_pngSize);
	if(seqImage.isNull()) seqImage = ImageCache::getFromMemory(BinaryData::seq_png, BinaryData::seq_pngSize);

	loadUI.reset(item->loadTrigger->createButtonUI());
	addAndMakeVisible(loadUI.get());

	directLoadUI.reset(item->directLoadTrigger->createButtonUI());
	addAndMakeVisible(directLoadUI.get());
	directLoadUI->customLabel = "Direct";
	directLoadUI->updateUIParams();

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

void SceneUI::paint(Graphics& g)
{
	BaseItemUI::paint(g);
	if (item->sequenceManager->items.size() > 0) g.drawImage(seqImage, seqRect);
	if (item->effectManager->items.size() > 0) g.drawImage(fxImage, fxRect);
}

void SceneUI::resizedInternalHeader(Rectangle<int>& r)
{
	directLoadUI->setBounds(r.removeFromRight(50).reduced(2));
	r.removeFromRight(2);
	loadUI->setBounds(r.removeFromRight(60).reduced(2));
	r.removeFromRight(2);
	loadProgressUI->setBounds(r.removeFromRight(80).reduced(2, 4));
	r.removeFromRight(2);
	seqRect = r.removeFromRight(r.getHeight()).reduced(1).toFloat();
	r.removeFromRight(2);
	fxRect = r.removeFromRight(r.getHeight()).reduced(1).toFloat();
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
		m.showMenuAsync(PopupMenu::Options(), [this](int result)
			{
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
		);
	}
}

void SceneUI::mouseDoubleClick(const MouseEvent& e)
{
	if (seqRect.contains(e.getEventRelativeTo(this).mouseDownPosition))
	{
		if (Sequence* s = item->sequenceManager->items.getFirst())
		{
			if (TimeMachineView* v = ShapeShifterManager::getInstance()->getContentForType<TimeMachineView>())
			{
				v->setSequence(s);
			}
			return;
		}
	}

	BaseItemUI::mouseDoubleClick(e);
}

void SceneUI::itemDropped(const DragAndDropTarget::SourceDetails& details)
{
	String dataType = details.description.getProperty("dataType", "");
	if (dataType == "Container")
	{
		if (BaseItemEditor* bi = dynamic_cast<BaseItemEditor*>(details.sourceComponent.get()))
		{
			if (Effect* e = dynamic_cast<Effect*>(bi->item))
			{
				item->effectManager->addItemFromData(e->getJSONData());
			}
		}
		else if (GenericManagerEditor<Effect>* gi = dynamic_cast<GenericManagerEditor<Effect>*>(details.sourceComponent.get()))
		{
			item->effectManager->loadJSONData(gi->manager->getJSONData());
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
