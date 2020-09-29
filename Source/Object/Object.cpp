/*
  ==============================================================================

    Object.cpp
    Created: 26 Sep 2020 10:02:32am
    Author:  bkupe

  ==============================================================================
*/

#include "Object.h"
#include "Interface/InterfaceManager.h"
#include "Object/Component/components/intensity/IntensityComponent.h"

Object::Object(var params) :
    BaseItem(params.getProperty("name", "Object")),
    objectType(params.getProperty("type", "Object").toString()),
    objectData(params)
{
    saveAndLoadRecursiveData = true;
   

    itemDataType = "Object";

    File iconFile = File(params.getProperty("path","")).getChildFile("icon.png");
    if (iconFile.existsAsFile()) customThumbnailPath = iconFile;


    targetInterface = addTargetParameter("Interface", "The interface to link this object to", InterfaceManager::getInstance());
    targetInterface->targetType = TargetParameter::CONTAINER;
    targetInterface->maxDefaultSearchLevel = 0;

    if (InterfaceManager::getInstance()->items.size() == 1) targetInterface->setValueFromTarget(InterfaceManager::getInstance()->items[0]);

    var cData = params.getProperty("components", var());
    if(cData.isObject())
    {
        NamedValueSet cProps = cData.getDynamicObject()->getProperties();
        for (auto& cp : cProps) componentManager.addComponentFromDefinition(cp.name, cp.value);
    }

    if (IntensityComponent* ic = getComponent<IntensityComponent>())	slideManipParameter = ic->value;

    componentManager.userCanAddItemsManually = params.getProperty("isCustom", false);
    addChildControllableContainer(&componentManager);
}

Object::~Object()
{
}

void Object::rebuildInterfaceParams()
{
    if (interfaceParameters != nullptr)
    {
        var gData = interfaceParameters->getJSONData();
        if (!gData.isVoid()) interfaceGhostData = gData;
        removeChildControllableContainer(interfaceParameters.get());
    }

    if (targetInterface->targetContainer != nullptr)
    {
        interfaceParameters.reset(((Interface*)targetInterface->targetContainer.get())->getInterfaceParams());
        addChildControllableContainer(interfaceParameters.get(), false, 0);
    }
    else interfaceParameters.reset();

    if (interfaceParameters != nullptr && !interfaceGhostData.isVoid()) interfaceParameters->loadJSONData(interfaceGhostData);
}

void Object::onContainerParameterChangedInternal(Parameter* p)
{
    BaseItem::onContainerParameterChangedInternal(p);
    if (p == targetInterface)
    {
        rebuildInterfaceParams();
    }
}

void Object::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
    BaseItem::onControllableFeedbackUpdateInternal(cc, c);

    if (c->type != Parameter::TRIGGER)
    {
        if (ObjectComponent* oc = c->getParentAs<ObjectComponent>())
        {
            if (Interface* i = dynamic_cast<Interface*>(targetInterface->targetContainer.get()))
            {
                i->updateValuesFromParameter(this, oc, (Parameter*)c);
            }
        }
    }
}
