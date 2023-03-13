/*
  ==============================================================================

    ObjectManager.cpp
    Created: 26 Sep 2020 10:02:28am
    Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

juce_ImplementSingleton(ObjectManager);

ObjectManager::ObjectManager() :
    BaseManager("Objects"),
    Thread("ObjectManager"),
    customParams("Custom Parameters", false,  false, true, true)
{
    itemDataType = "Object";
    selectItemWhenCreated = true;

    managerFactory = &factory;

    gridThumbSize = addIntParameter("Thumb Size", "Size of thumbnails in grid view", 96, 32, 256);
    defaultFlashValue = addFloatParameter("Flash Value", "Flash Value", .5f, 0, 1);
    blackOut = addBoolParameter("Black Out", "Force 0 on all computed values", false);
    updateRate = addIntParameter("Update Rate", "General update rate", 50, 1, 200);
    filterActiveInScene = addBoolParameter("Show Only active", "Show only active objects in scene", false);
    lockUI = addBoolParameter("Lock UI", "If checked, all objects will be locked", false);


    addChildControllableContainer(&customParams);

    File f = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile(String(ProjectInfo::projectName) + "/objects");
    if (!f.isDirectory())
    {
        downloadObjects();
    }
    else
    {
        updateFactoryDefinitions();
    }

    startThread();
}

ObjectManager::~ObjectManager()
{
    stopThread(1000);
}


void ObjectManager::itemAdded(GenericControllableItem*)
{
    objectManagerListeners.call(&ObjectManagerListener::customParamsChanged, this);
}

void ObjectManager::itemRemoved(GenericControllableItem*)
{
    objectManagerListeners.call(&ObjectManagerListener::customParamsChanged, this);
}

void ObjectManager::downloadObjects()
{
    LOG("Downloading objects...");
    downloadURL = URL("http://benjamin.kuperberg.fr/blux/download/objects.zip");
    File f = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile(String(ProjectInfo::projectName) + "/objects.zip");
    downloadTask = downloadURL.downloadToFile(f, URL::DownloadTaskOptions().withListener(this));
    if (downloadTask == nullptr)
    {
        LOGERROR("Error while downloading objects,\ntry downloading it directly from the website.");
    }
}

void ObjectManager::updateFactoryDefinitions()
{

    factory.defs.clear();
    Image img = ImageCache::getFromMemory(BinaryData::icon128_png, BinaryData::icon128_pngSize);

    var customParams(new DynamicObject());
    //customParams.getDynamicObject()->setProperty("canCustomize", true);
    customParams.getDynamicObject()->setProperty("isCustom", true);
    factory.defs.add(Factory<Object>::Definition::createDef("", "Custom", &Object::create, customParams)->addIcon(img));

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
    controllableContainers.move(controllableContainers.indexOf(&customParams), 0);
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

void ObjectManager::onContainerParameterChanged(Parameter* p)
{
    if (p == lockUI) for (auto& i : items) i->isUILocked->setValue(lockUI->boolValue());
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
        
        objectManagerListeners.call(&ObjectManagerListener::updateStart);
        for (auto& i : InterfaceManager::getInstance()->items) i->prepareSendValues(); //interfaces should listen to updateStart and updateFinish
        
        items.getLock().enter();
        for (auto& o : items)  o->checkAndComputeComponentValuesIfNeeded();
        items.getLock().exit();
        

        objectManagerListeners.call(&ObjectManagerListener::updateFinish);
        for (auto& i : InterfaceManager::getInstance()->items) i->finishSendValues(); //interfaces should listen to updateStart and updateFinish

        long millisAfter = Time::getMillisecondCounter();
        long millisToSleep = jmax<long>(1, 1000.0/updateRate->intValue()-(millisAfter-millisBefore));
        sleep((int)millisToSleep);
    }
}

void ObjectManager::progress(URL::DownloadTask* task, int64 downloaded, int64 total)
{
    int percent = (int)(downloaded * 100 / total);
    LOG("Downloading objects..." << percent << "%");
}

void ObjectManager::finished(URL::DownloadTask* task, bool success)
{
    if (!success)
    {
        LOGERROR("Error downloading objects");
        return;
    }

    File f = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile(String(ProjectInfo::projectName) + "/objects.zip");
    ZipFile z(f);
    if (!z.uncompressTo(f.getParentDirectory()))
    {
        LOGERROR("Error unzipping objects archive");
        return;
    }


    LOG(z.getNumEntries() << "objects downloaded.");
    f.deleteFile();

    updateFactoryDefinitions();
}

var ObjectManager::getJSONData()
{
    var data = BaseManager::getJSONData();
    data.getDynamicObject()->setProperty(customParams.shortName, customParams.getJSONData());
    return data;
}

void ObjectManager::loadJSONDataManagerInternal(var data)
{
    customParams.loadJSONData(data.getProperty(customParams.shortName, var()));
    BaseManager::loadJSONDataManagerInternal(data);
}

ObjectManagerCustomParams* ObjectManager::getCustomParams() 
{
    return new ObjectManagerCustomParams(this); 
}



ObjectManagerCustomParams::ObjectManagerCustomParams(ObjectManager* om) :
    ControllableContainer("Custom Parameters"),
    om(om)
{
    if (om != nullptr) om->addObjectManagerListener(this);
    rebuildCustomParams();
}


ObjectManagerCustomParams::~ObjectManagerCustomParams()
{
    if (om != nullptr) om->removeObjectManagerListener(this);
    om = nullptr;
}


void ObjectManagerCustomParams::customParamsChanged(ObjectManager*)
{
    rebuildCustomParams();
}

void ObjectManagerCustomParams::rebuildCustomParams()
{
    if (om == nullptr) return;

    var oldData = getJSONData();
    clear();

    for (auto& gci : om->customParams.items)
    {
        if (gci->controllable->type == Controllable::TRIGGER) continue;

        if (Parameter* p = ControllableFactory::createParameterFrom((Parameter*)gci->controllable, true, true))
        {
            p->canBeDisabledByUser = true;
            p->setEnabled(false);
            addParameter(p);
        }
    }

    loadJSONData(oldData);
}

var ObjectManagerCustomParams::getParamValueFor(WeakReference<Parameter> p)
{
    return getParamValueForName(p->shortName);
}

var ObjectManagerCustomParams::getParamValueForName(const String& name)
{
    if (Parameter* p = getActiveCustomParamForName(name)) return p->getValue();
    jassertfalse;
    return var();
}

var ObjectManagerCustomParams::getParamValues()
{
    Array<WeakReference<Parameter>> params = getAllParameters();

    var values(new DynamicObject());
    for (auto& p : params)
    {
        Parameter* targetP = p;
        if (!p->enabled)
        {
            if (GenericControllableItem* gci = om->customParams.getItemWithName(p->shortName)) targetP = (Parameter*)gci->controllable;
            else targetP = nullptr;
        }

        if (targetP != nullptr) values.getDynamicObject()->setProperty(targetP->shortName, targetP->getValue());

    }
    return values;
}


Parameter * ObjectManagerCustomParams::getActiveParamFor(WeakReference<Parameter> p)
{
    return getActiveCustomParamForName(p->shortName);
}


Parameter* ObjectManagerCustomParams::getActiveCustomParamForName(const String& name)
{
    if (Parameter* p = getParameterByName(name))
    {
        if (p->enabled) return p;
    }

    if (GenericControllableItem* gci = ObjectManager::getInstance()->customParams.getItemWithName(name)) return ((Parameter*)gci->controllable);

    return nullptr;
}


// SUB OBJECT


SubObjectManager::SubObjectManager() :
    BaseManager("Sub-Objects")
{
    managerFactory = &ObjectManager::getInstance()->factory;
    selectItemWhenCreated = false;
}

SubObjectManager::~SubObjectManager()
{
}
