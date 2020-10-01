/*
  ==============================================================================

    ObjectManager.cpp
    Created: 26 Sep 2020 10:02:28am
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectManager.h"

juce_ImplementSingleton(ObjectManager);

ObjectManager::ObjectManager() :
    BaseManager("Objects"),
    Thread("Object compute")
{
    managerFactory = &factory;
    itemDataType = "Object";

    gridThumbSize = addIntParameter("Thumb Size", "Size of thumbnails in grid view", 64, 32, 128);
    gridThumbSize->hideInEditor = true;
    
    startThread();

    updateFactoryDefinitions();
}

ObjectManager::~ObjectManager()
{
    stopThread(1000);
}


void ObjectManager::updateFactoryDefinitions()
{
    File objectsFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(String(ProjectInfo::projectName)+"/objects");
    Array<File> objectsList = objectsFolder.findChildFiles(File::findDirectories, false);
    for (auto& of : objectsList)
    {
        File defFile = of.getChildFile("definition.json");
        if (!defFile.existsAsFile())
        {
            LOGWARNING("Object " << of.getFileName() << " definition file not found");
            continue;
        }

        var def = JSON::parse(defFile);
        if (!def.isObject() || !def.hasProperty("name"))
        {
            LOGWARNING("Object " << of.getFileName() << "definition file is not valid");
            continue;
        }
        
        def.getDynamicObject()->setProperty("path", of.getFullPathName());

        Image img = ImageCache::getFromFile(of.getChildFile("icon.png"));
        factory.defs.add(Factory<Object>::Definition::createDef(def.getProperty("menu","").toString(), def.getProperty("name","[noname]").toString(), &Object::create, def)->addIcon(img));
    }
}


void ObjectManager::addItemInternal(Object* o, var data)
{
    o->addObjectListener(this);
    if (!isCurrentlyLoadingData) o->globalID->setValue(getFirstAvailableObjectID());
}

void ObjectManager::removeItemInternal(Object* o)
{
    o->removeObjectListener(this);
}

int ObjectManager::getFirstAvailableObjectID()
{
    Array<int> ids;
    for (auto& o : items) ids.add(o->globalID->intValue());

    int id = 0;
    while (ids.contains(id)) id++;
    return id;
}

Object* ObjectManager::getObjectWithID(int id, Object* excludeObject)
{
    for (auto& o : items) if (o != excludeObject && o->globalID->intValue() == id) return o;
    return nullptr;
}

void ObjectManager::objectIDChanged(Object* o, int previousID)
{
    if (isCurrentlyLoadingData) return;
    Object* to = getObjectWithID(o->globalID->intValue(), o);
    if (to != nullptr) to->globalID->setValue(previousID);
}

void ObjectManager::saveSceneData(var &sceneData)
{
    for (auto& o : items) o->saveSceneData(sceneData);
}

void ObjectManager::run()
{
    while (!threadShouldExit()) 
    {
        long millisBefore = Time::getMillisecondCounter();
        items.getLock().enter();
        for (auto& o : items)  o->checkAndComputeComponentValuesIfNeeded();
        items.getLock().exit();
        long millisAfter = Time::getMillisecondCounter();

        long millisToSleep = jmax<long>(1, 30-(millisAfter-millisBefore));
        sleep(millisToSleep);
    }
}
