/*
  ==============================================================================

	BluxEngine.cpp
	Created: 26 Sep 2020 10:01:11am
	Author:  bkupe

  ==============================================================================
*/

#include "BluxEngine.h"
#include "Object/ObjectIncludes.h"
#include "Interface/InterfaceIncludes.h"
#include "Effect/EffectIncludes.h"
#include "Sequence/SequenceIncludes.h"
#include "Scene/SceneIncludes.h"
#include "Audio/AudioManager.h"
#include "Common/CommonIncludes.h"

BluxEngine::BluxEngine() :
	Engine("Blux", ".blux")
{
	mainEngine = this;
	addChildControllableContainer(ObjectManager::getInstance());
	addChildControllableContainer(InterfaceManager::getInstance());
	addChildControllableContainer(GroupManager::getInstance());
	addChildControllableContainer(SceneManager::getInstance());
	addChildControllableContainer(GlobalEffectManager::getInstance());
	addChildControllableContainer(GlobalSequenceManager::getInstance());
	addChildControllableContainer(StageLayoutManager::getInstance());

	GlobalSettings::getInstance()->addChildControllableContainer(BluxSettings::getInstance());
	GlobalSettings::getInstance()->addChildControllableContainer(AudioManager::getInstance());

	ColorSourceFactory::getInstance(); //avoid initialization in other than main thread;

	convertURL = "http://benjamin.kuperberg.fr/blux/releases/convert.php";
	breakingChangesVersions.add("1.0.0b4");
	breakingChangesVersions.add("1.2.0b1");
	breakingChangesVersions.add("1.2.0");

	initVizServer();
}

BluxEngine::~BluxEngine()
{
	isClearing = true;
	ObjectManager::getInstance()->clear();
	ObjectManager::deleteInstance();
	GroupManager::deleteInstance();
	SceneManager::deleteInstance();
	GlobalEffectManager::deleteInstance();
	GlobalSequenceManager::deleteInstance();
	StageLayoutManager::deleteInstance();
	ColorSourceLibrary::deleteInstance();

	TimedEffectHiResTimer::getInstance()->stopTimer();
	TimedEffectHiResTimer::deleteInstance();

	InterfaceManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();

	ActionFactory::deleteInstance();
	EffectFactory::deleteInstance();
	EffectBlockFactory::deleteInstance();

	AudioManager::deleteInstance();
	BluxSettings::deleteInstance();

	ColorSourceFactory::deleteInstance();

	MIDIManager::deleteInstance();

	if (vizServer != nullptr) vizServer->stop();
}

void BluxEngine::initVizServer()
{
	if (vizServer != nullptr && vizServer->isConnected)
	{
		NLOG(niceName, "Stopping Server");
		//vizServer->removeWebSocketListener(this);
		vizServer->stop();
	}

	vizServer.reset();

	vizServer.reset(new SimpleWebSocketServer());
	//vizServer->addWebSocketListener(this);

	vizServer->rootPath = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(ProjectInfo::projectName + String("/server"));

	int port = 6080;
	vizServer->start(port);
	vizServer->addWebSocketListener(this);

	if (vizServer->isConnected)
	{
		NLOG(niceName, "Server is running on port " << port);
	}
	else
	{
		NLOGERROR(niceName, "Error starting server on port " << port);
	}
}

void BluxEngine::createNewGraphInternal()
{
	InterfaceManager::getInstance()->addItem(new DMXInterface());
}

void BluxEngine::connectionOpened(const String& id)
{
	sendAllData(id);
}

void BluxEngine::messageReceived(const String& id, const String& message)
{

}

void BluxEngine::sendAllData(const String& id)
{
	var data = getVizData();
	var msgData(new DynamicObject());
	msgData.getDynamicObject()->setProperty("type", "setup");
	msgData.getDynamicObject()->setProperty("data", data);
	sendServerMessage(JSON::toString(msgData), id);
}

void BluxEngine::sendControllableData(Controllable* c)
{
	if (c->type == Controllable::TRIGGER) return;
	Parameter* p = (Parameter*)c;

	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("controlAddress", p->getControlAddress(this));
	data.getDynamicObject()->setProperty("value", p->getValue());

	;	var msgData(new DynamicObject());
	msgData.getDynamicObject()->setProperty("type", "controllable");
	msgData.getDynamicObject()->setProperty("data", data);
	sendServerMessage(JSON::toString(msgData));
}

void BluxEngine::sendServerMessage(const String message, const String& id)
{
	if (id.isNotEmpty()) vizServer->sendTo(message, id);
	else vizServer->send(message);
}

var BluxEngine::getVizData()
{
	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("objects", ObjectManager::getInstance()->getVizData());
	return data;
}

void BluxEngine::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	Engine::onControllableFeedbackUpdate(cc, c);
	if (!isClearing && ObjectManager::getInstanceWithoutCreating() != nullptr && cc == ObjectManager::getInstance()) sendControllableData(c);
}

void BluxEngine::clearInternal()
{
	ObjectManager::getInstance()->clear();
	GroupManager::getInstance()->clear();
	SceneManager::getInstance()->clear();
	GlobalEffectManager::getInstance()->clear();
	GlobalSequenceManager::getInstance()->clear();
	StageLayoutManager::getInstance()->clear();
	ColorSourceLibrary::getInstance()->clear();

	InterfaceManager::getInstance()->clear();
}

String BluxEngine::getMinimumRequiredFileVersion()
{
	return "1.0.0b1";
}

var BluxEngine::getJSONData()
{
	var data = Engine::getJSONData();

	//save here
	data.getDynamicObject()->setProperty(ColorSourceLibrary::getInstance()->shortName, ColorSourceLibrary::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(ObjectManager::getInstance()->shortName, ObjectManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(InterfaceManager::getInstance()->shortName, InterfaceManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(GroupManager::getInstance()->shortName, GroupManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(SceneManager::getInstance()->shortName, SceneManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(GlobalEffectManager::getInstance()->shortName, GlobalEffectManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(GlobalSequenceManager::getInstance()->shortName, GlobalSequenceManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty(StageLayoutManager::getInstance()->shortName, StageLayoutManager::getInstance()->getJSONData());
	return data;

}

void BluxEngine::loadJSONDataInternalEngine(var data, ProgressTask* loadingTask)
{
	ProgressTask* bluxTask = loadingTask->addTask("Blux");

	bluxTask->start();

	InterfaceManager::getInstance()->loadJSONData(data.getProperty(InterfaceManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.1f);

	ColorSourceLibrary::getInstance()->loadJSONData(data.getProperty(ColorSourceLibrary::getInstance()->shortName, var()));
	bluxTask->setProgress(.15f);

	ObjectManager::getInstance()->loadJSONData(data.getProperty(ObjectManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.2f);

	GroupManager::getInstance()->loadJSONData(data.getProperty(GroupManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.3f);

	SceneManager::getInstance()->loadJSONData(data.getProperty(SceneManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.4f);

	GlobalEffectManager::getInstance()->loadJSONData(data.getProperty(GlobalEffectManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.5f);

	GlobalSequenceManager::getInstance()->loadJSONData(data.getProperty(GlobalSequenceManager::getInstance()->shortName, var()));
	bluxTask->setProgress(.6f);

	StageLayoutManager::getInstance()->loadJSONData(data.getProperty(StageLayoutManager::getInstance()->shortName, var()));
	bluxTask->setProgress(1);

	bluxTask->end();
}

juce_ImplementSingleton(BluxSettings)

BluxSettings::BluxSettings() :
	ControllableContainer("Blux Settings")
{
	defaultSceneLoadTime = addFloatParameter("Default Scene Load Time", "The default load time to set the scenes to on creation", 1, 0);
	defaultSceneLoadTime->defaultUI = FloatParameter::TIME;
}

BluxSettings::~BluxSettings()
{
}
