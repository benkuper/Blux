/*
  ==============================================================================

    StageLayout.cpp
    Created: 6 Oct 2020 9:37:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "StageLayout.h"
#include "Object/ObjectManager.h"

StageLayout::StageLayout() :
    BaseItem("Layout", false)
{
    saveLayout();
}

StageLayout::~StageLayout()
{
}

void StageLayout::saveLayout()
{
    layoutData = var(new DynamicObject());
    for (auto& o : ObjectManager::getInstance()->items) layoutData.getDynamicObject()->setProperty(o->shortName, o->stagePosition->getValue());
}

var StageLayout::getJSONData()
{
    var data = BaseItem::getJSONData();
    data.getDynamicObject()->setProperty("layoutData", layoutData);
    return data;
}

void StageLayout::loadJSONDataItemInternal(var data)
{
    layoutData = data.getProperty("layoutData", var());
}