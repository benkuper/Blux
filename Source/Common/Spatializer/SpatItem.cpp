/*
  ==============================================================================

	SpatItem.cpp
	Created: 24 Dec 2023 10:04:48am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"
#include "Object/ObjectIncludes.h"

SpatItem::SpatItem(var params)
{
	setHasCustomColor(true);

	viewUIPosition->setPoint(-1, -1);
	viewUISize->setPoint(2, 2);
	viewUIPosition->hideInEditor = false;
	viewUISize->hideInEditor = false;
}

SpatItem::~SpatItem()
{

}

Point<float> SpatItem::getObjectPosition(Object* o)
{
	if (o == nullptr) return Point<float>(0, 0);

	Vector3D<float> oPos = o->stagePosition->getVector();
	Point<float> pos(oPos.x, oPos.z);

	return (pos - viewUIPosition->getPoint()) / viewUISize->getPoint();
}
