/*
  ==============================================================================

	ObjectChainVizUI.cpp
	Created: 30 Oct 2020 4:35:09pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

ObjectChainVizUI::ObjectChainVizUI(Object* o, ComponentType ct, ChainVizTarget::ChainVizType type) :
	BaseItemChainVizComponent(o, o, ct, type)
{
	if (ObjectComponent* ic = o->getComponentForType(ct))
	{
		Parameter* p = type == ChainVizTarget::ChainVizType::OBJECT_START ? ic->computedParamMap[ic->mainParameter] : ic->mainParameter;
		if (p != nullptr)
		{
			paramUI.reset((ParameterUI*)p->createDefaultUI());
			paramUI->showLabel = false;
			addAndMakeVisible(paramUI.get());
		}
	}
}


ObjectChainVizUI::~ObjectChainVizUI()
{
}

void ObjectChainVizUI::resized()
{
	BaseItemChainVizComponent::resized();
	Rectangle<int> r = getLocalBounds().reduced(4);

	if (paramUI != nullptr)
	{
		paramUI->setBounds(r.removeFromBottom(16));
	}
}
