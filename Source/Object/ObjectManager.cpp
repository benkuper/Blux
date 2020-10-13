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

    gridThumbSize = addIntParameter("Thumb Size", "Size of thumbnails in grid view", 128, 32, 256);
    
    defaultFlashValue = addFloatParameter("Flash Value", "Flash Value", .5f, 0, 1);

    blackOut = addBoolParameter("Black Out", "Force 0 on all computed values", false);

    filterActiveInScene = addBoolParameter("Show Only active", "Show only active objects in scene", false);
    startThread();

    updateFactoryDefinitions();
}

ObjectManager::~ObjectManager()
{
    stopThread(1000);
}


void ObjectManager::updateFactoryDefinitions()
{
    factory.defs.clear();

    File objectsFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(String(ProjectInfo::projectName)+"/objects");
    Array<File> objectsList = objectsFolder.findChildFiles(File::findDirectories, false);

    for (int i = objectsList.size() - 1; i >= 0; i--) //reverse loop because directory listing is inverted
    {
        File of = objectsList[i];
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
    if (!isCurrentlyLoadingData) o->globalID->setValue(getFirstAvailableObjectID(o));
}

void ObjectManager::removeItemInternal(Object* o)
{
    o->removeObjectListener(this);
}

int ObjectManager::getFirstAvailableObjectID(Object* excludeObject)
{
    Array<int> ids;
    for (auto& o : items) if(o != excludeObject) ids.add(o->globalID->intValue());

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

var ObjectManager::getSceneData()
{
    var data(new DynamicObject());
    for (auto& o : items) data.getDynamicObject()->setProperty(o->shortName, o->getSceneData());
    return data;
}

void ObjectManager::updateSceneData(var& sceneData)
{
}

void ObjectManager::lerpFromSceneData(var startData, var endData, float weight)
{
    for (auto& o : items) o->lerpFromSceneData(startData.getProperty(o->shortName,var()), endData.getProperty(o->shortName, var()), weight);
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
        sleep((int)millisToSleep);
    }
}
