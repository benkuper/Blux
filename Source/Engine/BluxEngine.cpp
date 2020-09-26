/*
  ==============================================================================

	BluxEngine.cpp
	Created: 26 Sep 2020 10:01:11am
	Author:  bkupe

  ==============================================================================
*/

#include "BluxEngine.h"
#include "Object/ObjectManager.h"

BluxEngine::BluxEngine() :
	Engine("Blux", ".blux")
{
	mainEngine = this;
	addChildControllableContainer(ObjectManager::getInstance());
}

BluxEngine::~BluxEngine()
{
	ObjectManager::deleteInstance();
}

void BluxEngine::clearInternal()
{
	ObjectManager::getInstance()->clear();
}

var BluxEngine::getJSONData()
{
	var data = Engine::getJSONData();

	//save here
	var pData = ProjectSettings::getInstance()->getJSONData();
	if (!pData.isVoid() && pData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("projectSettings", pData);

	var oData = ObjectManager::getInstance()->getJSONData();
	if (!oData.isVoid() && oData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ObjectManager::getInstance()->shortName, oData);

	return data;

}

void BluxEngine::loadJSONDataInternalEngine(var data, ProgressTask* loadingTask)
{
	ProgressTask* bluxTask = loadingTask->addTask("Blux");

	bluxTask->start();

	ObjectManager::getInstance()->loadJSONData(data.getProperty(ObjectManager::getInstance()->shortName, var()));

	bluxTask->setProgress(1);
	bluxTask->end();

}
