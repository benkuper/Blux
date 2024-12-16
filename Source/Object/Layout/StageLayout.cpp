/*
  ==============================================================================

    StageLayout.cpp
    Created: 6 Oct 2020 9:37:41pm
    Author:  bkupe

  ==============================================================================
*/

StageLayout::StageLayout() :
    BaseItem("Layout", false)
{
    saveLayout();
}

StageLayout::~StageLayout()
{
}

void StageLayout::loadLayout()
{
    for (auto& o : ObjectManager::getInstance()->items)
    {
        o->stagePosition->setVector(getObjectPosition(o));
    }
}

void StageLayout::saveLayout()
{
    layoutData = var(new DynamicObject());
    for (auto& o : ObjectManager::getInstance()->items) layoutData.getDynamicObject()->setProperty(o->shortName, o->stagePosition->getValue());
}

Vector3D<float> StageLayout::getObjectPosition(Object* o)
{
    var p = layoutData.getProperty(o->shortName, var());
    if (p.isVoid()) return Vector3D<float>();
    return Vector3D<float>(p[0], p[1], p[2]);
}

var StageLayout::getJSONData(bool includeNonOverriden)
{
    var data = BaseItem::getJSONData(includeNonOverriden);
    data.getDynamicObject()->setProperty("layoutData", layoutData);
    return data;
}

void StageLayout::loadJSONDataItemInternal(var data)
{
    layoutData = data.getProperty("layoutData", var());
}