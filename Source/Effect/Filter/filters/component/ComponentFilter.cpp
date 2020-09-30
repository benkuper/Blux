/*
  ==============================================================================

    ComponentFilter.cpp
    Created: 30 Sep 2020 2:56:39pm
    Author:  bkupe

  ==============================================================================
*/

#include "ComponentFilter.h"
#include "Object/Component/ComponentManager.h"

ComponentFilter::ComponentFilter() :
    Filter(getTypeString()),
    components("Components")
{
    mode = addEnumParameter("Mode", "Filtering mode");
    mode->addOption("Include", true)->addOption("Exclude", false);

    components.userCanAddControllables = true;
    components.customUserCreateControllableFunc = std::bind(&ComponentFilter::createComponentEnum, this, std::placeholders::_1);
    addChildControllableContainer(&components);
}

ComponentFilter::~ComponentFilter()
{
}

EnumParameter * ComponentFilter::createComponentEnum(ControllableContainer* cc)
{
    EnumParameter* ep = cc->addEnumParameter("Type", "Component Type to filter");
    for (auto& d : ComponentFactory::getInstance()->defs)
    {
        ep->addOption(d->type, d->type);
    }
    ep->isRemovableByUser = true;

    return ep;
}

var ComponentFilter::getJSONData()
{
    var data = Filter::getJSONData();
    var cData;
    for (auto& c : components.controllables)  cData.append(((EnumParameter*)c)->getValueData());
    data.getDynamicObject()->setProperty("components", cData);
    return data;
}

void ComponentFilter::loadJSONDataInternal(var data)
{
    Filter::loadJSONDataInternal(data);

    var cData = data.getProperty("components", var());
    for (int i = 0; i < cData.size(); i++)
    {
        EnumParameter* ep = createComponentEnum(&components);
        ep->setValueWithData(cData[i]);
    }
}
