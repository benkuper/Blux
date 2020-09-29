/*
  ==============================================================================

	BluxEngine.cpp
	Created: 26 Sep 2020 10:01:11am
	Author:  bkupe

  ==============================================================================
*/

#include "BluxEngine.h"
#include "Object/ObjectManager.h"
#include "Interface/InterfaceManager.h"
#include "Common/DMX/DMXManager.h"
#include "Common/Serial/SerialManager.h"

BluxEngine::BluxEngine() :
	Engine("Blux", ".blux")
{
	mainEngine = this;
	addChildControllableContainer(ObjectManager::getInstance());
	addChildControllableContainer(InterfaceManager::getInstance());
}

BluxEngine::~BluxEngine()
{
	ObjectManager::deleteInstance();
	InterfaceManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();
}

void BluxEngine::clearInternal()
{
	ObjectManager::getInstance()->clear();
	InterfaceManager::getInstance()->clear();
}

var BluxEngine::getJSONData()
{
	var data = Engine::getJSONData();

	//save here
	var oData = ObjectManager::getInstance()->getJSONData();
	if (!oData.isVoid() && oData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ObjectManager::getInstance()->shortName, oData);

	var iData = InterfaceManager::getInstance()->getJSONData();
	if (!iData.isVoid() && iData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(InterfaceManager::getInstance()->shortName, iData);

	return data;

}

void BluxEngine::loadJSONDataInternalEngine(var data, ProgressTask* loadingTask)
{
	ProgressTask* bluxTask = loadingTask->addTask("Blux");

	bluxTask->start();

	InterfaceManager::getInstance()->loadJSONData(data.getProperty(InterfaceManager::getInstance()->shortName, var()));
	bluxTask->setProgress(1);
	bluxTask->end();

	ObjectManager::getInstance()->loadJSONData(data.getProperty(ObjectManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.5f);

}
