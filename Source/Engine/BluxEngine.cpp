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
#include "Group/GroupManager.h"

BluxEngine::BluxEngine() :
	Engine("Blux", ".blux")
{
	mainEngine = this;
	addChildControllableContainer(ObjectManager::getInstance());
	addChildControllableContainer(InterfaceManager::getInstance());
	addChildControllableContainer(GroupManager::getInstance());
}

BluxEngine::~BluxEngine()
{
	GroupManager::deleteInstance();
	ObjectManager::deleteInstance();
	InterfaceManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();
}

void BluxEngine::clearInternal()
{
	GroupManager::getInstance()->clear();
	ObjectManager::getInstance()->clear();
	InterfaceManager::getInstance()->clear();
}

var BluxEngine::getJSONData()
{
	var data = Engine::getJSONData();

	//save here
	data.getDynamicObject()->setProperty(ObjectManager::getInstance()->shortName, ObjectManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(InterfaceManager::getInstance()->shortName, InterfaceManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(GroupManager::getInstance()->shortName, GroupManager::getInstance()->getJSONData());

	return data;

}

void BluxEngine::loadJSONDataInternalEngine(var data, ProgressTask* loadingTask)
{
	ProgressTask* bluxTask = loadingTask->addTask("Blux");

	bluxTask->start();

	InterfaceManager::getInstance()->loadJSONData(data.getProperty(InterfaceManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.5f);

	ObjectManager::getInstance()->loadJSONData(data.getProperty(ObjectManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.7f);

	GroupManager::getInstance()->loadJSONData(data.getProperty(GroupManager::getInstance()->shortName, var()));

	bluxTask->setProgress(1);
	bluxTask->end();
}
