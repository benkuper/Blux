/*
  ==============================================================================

	ObjectChainVizUI.cpp
	Created: 30 Oct 2020 4:35:09pm
	Author:  bkupe

  ==============================================================================
*/

#include "Object/ObjectIncludes.h"

ObjectChainVizUI::ObjectChainVizUI(Object* o, ChainVizTarget::ChainVizType type) :
	BaseItemChainVizComponent(o, o, type)
{
	if (DimmerComponent* ic = o->getComponent<DimmerComponent>())
	{
		FloatParameter* sourceIntensity = (FloatParameter*)ic->computedParamMap[ic->value];
		FloatParameter* computedIntensity = (FloatParameter*)ic->computedParameters[0];
		FloatParameter* ip = (type == ChainVizTarget::ChainVizType::OBJECT_START) ? sourceIntensity : computedIntensity;
		if (ip != nullptr)
		{
			intensityUI.reset(ip->createSlider());
			addAndMakeVisible(intensityUI.get());
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

	if (intensityUI != nullptr)
	{
		intensityUI->setBounds(r.removeFromBottom(16));
	}
}
