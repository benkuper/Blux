/*
  ==============================================================================

	StageLayout2DView.cpp
	Created: 6 Oct 2020 9:49:35pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"
#include "Effect/Filter/filters/layout/ui/LayoutFilterStageView.h"

StageLayout2DView::StageLayout2DView(const String& name) :
	BaseManagerShapeShifterViewUI(name, ObjectManager::getInstance())
{
	animateItemOnAdd = false;

	canZoom = true;
	contentIsFlexible = true;

	useCheckersAsUnits = true;
	updatePositionOnDragMove = true;

	centerUIAroundPosition = true;

	iconSizeUI.reset(StageLayoutManager::getInstance()->iconSize->createSlider());
	iconSizeUI->useCustomBGColor = true;
	iconSizeUI->customBGColor = BG_COLOR.darker(.5f);
	addAndMakeVisible(iconSizeUI.get());

	showFiltersUI.reset(StageLayoutManager::getInstance()->showFilters->createToggle());
	addAndMakeVisible(showFiltersUI.get());

	lockObjectUIs.reset(StageLayoutManager::getInstance()->lockObjectUIs->createToggle());
	addAndMakeVisible(lockObjectUIs.get());

	lockFilterUIs.reset(StageLayoutManager::getInstance()->lockFilterUIs->createToggle());
	addAndMakeVisible(lockFilterUIs.get());

	showSpatUI.reset(StageLayoutManager::getInstance()->showSpat->createToggle());
	addAndMakeVisible(showSpatUI.get());

	filterStageView.reset(new LayoutFilterStageView());
	addChildComponent(filterStageView.get());
	filterStageView->setVisible(StageLayoutManager::getInstance()->showFilters->boolValue());

	spatManagerView.reset(new SpatManagerView(&ObjectManager::getInstance()->spatializer));
	addChildComponent(spatManagerView.get());
	spatManagerView->setVisible(StageLayoutManager::getInstance()->showSpat->boolValue());

	StageLayoutManager::getInstance()->addAsyncContainerListener(this);
	manager->addAsyncContainerListener(this);
	setShowSearchBar(true);

	addExistingItems();
}

StageLayout2DView::~StageLayout2DView()
{
	if (!inspectable.wasObjectDeleted()) manager->removeAsyncContainerListener(this);

	StageLayoutManager::getInstance()->removeAsyncContainerListener(this);
}

void StageLayout2DView::addItemUIInternal(Object2DView* ui)
{
	if (filterStageView->isVisible()) filterStageView->toFront(false);
	ui->item->isUILocked->setValue(StageLayoutManager::getInstance()->lockObjectUIs->boolValue());
}

void StageLayout2DView::setPreviewData(var data)
{
	for (auto& i : itemsUI) i->setPreviewData(data.getProperty(i->item->shortName, var()));
}


void StageLayout2DView::resized()
{
	BaseManagerShapeShifterViewUI::resized();

	Rectangle<int> r = getLocalBounds().removeFromTop(headerSize);
	r.setLeft(searchBar->getRight() + 8);

	iconSizeUI->setBounds(r.removeFromLeft(200).reduced(3));
	r.removeFromLeft(8);
	showFiltersUI->setBounds(r.removeFromLeft(100).reduced(3));
	r.removeFromLeft(8);
	lockObjectUIs->setBounds(r.removeFromLeft(100).reduced(3));
	r.removeFromLeft(8);
	lockFilterUIs->setBounds(r.removeFromLeft(100).reduced(3));
	r.removeFromLeft(8);
	showSpatUI->setBounds(r.removeFromLeft(100).reduced(3));

	if (filterStageView->isVisible())
	{
		filterStageView->setBounds(getLocalBounds());
		filterStageView->setBoundsInView(getViewBounds(getLocalBounds()));
		filterStageView->toFront(false);
	}

	if (filterStageView->isVisible())
	{
		spatManagerView->setBounds(getLocalBounds());
		spatManagerView->manager->viewOffset = manager->viewOffset;
		spatManagerView->setViewZoom(manager->viewZoom);
		spatManagerView->toFront(false);
	}
}


void StageLayout2DView::newMessage(const ContainerAsyncEvent& e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
		if (e.targetControllable == StageLayoutManager::getInstance()->showFilters)
		{
			filterStageView->setVisible(StageLayoutManager::getInstance()->showFilters->boolValue());
		}
		else if (e.targetControllable == StageLayoutManager::getInstance()->showSpat)
		{
			spatManagerView->setVisible(StageLayoutManager::getInstance()->showSpat->boolValue());
		}
		else if (e.targetControllable == StageLayoutManager::getInstance()->iconSize)
		{
			for (auto& ui : itemsUI)
			{
				float s = StageLayoutManager::getInstance()->iconSize->floatValue();
				ui->setSize(s, s);
				updateViewUIPosition(ui);
			}
		}
		else if (e.targetControllable == StageLayoutManager::getInstance()->lockObjectUIs)
		{
			bool locked = StageLayoutManager::getInstance()->lockObjectUIs->boolValue();
			for (auto& ui : itemsUI)
			{
				ui->item->isUILocked->setValue(locked);
			}
		}
		else if (e.targetControllable == StageLayoutManager::getInstance()->lockFilterUIs)
		{
			bool locked = StageLayoutManager::getInstance()->lockFilterUIs->boolValue();
			for (auto& ui : filterStageView->filterUIs)
			{
				ui->setInterceptsMouseClicks(!locked, !locked);
			}
		}
		else if (e.targetControllable == manager->showIconForColor) for (auto& i : itemsUI) i->updateUI();

		break;
	default:
		break;
	}
}
