/*
  ==============================================================================

    Object.cpp
    Created: 26 Sep 2020 10:02:32am
    Author:  bkupe

  ==============================================================================
*/

#include "Object.h"

Object::Object(var params) :
    BaseItem(params.getProperty("name", "Object")),
    objectType(params.getProperty("type", "Object").toString()),
    objectData(params)
{
    saveAndLoadRecursiveData = true;
   
    itemDataType = "Object";

    File iconFile = File(params.getProperty("path","")).getChildFile("icon.png");
    if (iconFile.existsAsFile()) customThumbnailPath = iconFile;

    var cData = params.getProperty("components", var());
    if(cData.isObject())
    {
        NamedValueSet cProps = cData.getDynamicObject()->getProperties();
        for (auto& cp : cProps) componentManager.addComponentFromDefinition(cp.name, cp.value);
    }

    componentManager.userCanAddItemsManually = params.getProperty("isCustom", false);
    addChildControllableContainer(&componentManager);
}

Object::~Object()
{
}
