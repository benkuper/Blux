/*
  ==============================================================================

	BluxEngine.cpp
	Created: 26 Sep 2020 10:01:11am
	Author:  bkupe

  ==============================================================================
*/


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

	convertURL = "http://benjamin.kuperberg.fr/blux/releases/convert.php";
	breakingChangesVersions.add("1.0.0b4");
}

BluxEngine::~BluxEngine()
{
	isClearing = true;
	ObjectManager::deleteInstance();
	GroupManager::deleteInstance();
	SceneManager::deleteInstance();
	GlobalEffectManager::deleteInstance();
	GlobalSequenceManager::deleteInstance();
	StageLayoutManager::deleteInstance();
	ColorSourceLibrary::deleteInstance();

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
