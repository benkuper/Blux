/*
  ==============================================================================

	EffectChainVizUI.cpp
	Created: 30 Oct 2020 4:35:34pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

EffectChainVizUI::EffectChainVizUI(Effect* e, Object* o, ComponentType ct, ChainVizTarget::ChainVizType type) :
	BaseItemChainVizComponent(e, o, ct, type),
	effect(e),
	effectRef(e)
{
	object->addInspectableListener(this);

	weightUI.reset(e->weight->createSlider());
	addAndMakeVisible(weightUI.get());

	if (ObjectComponent* c = o->getComponentForType(ct))
	{
		param.reset(ControllableFactory::createParameterFrom(c->mainParameter, true));
		if (param != nullptr)
		{
			param->setControllableFeedbackOnly(true);
			paramUI.reset((ParameterUI*)param->createDefaultUI());
			paramUI->showLabel = false;
			addAndMakeVisible(paramUI.get());

			effect->registerVizFeedback(param.get(), c->mainParameter);
		}
	}


	startTimerHz(30);
	timerCallback(); //show directly
}

EffectChainVizUI::~EffectChainVizUI()
{
	if (object != nullptr) object->removeInspectableListener(this);
	if (!effectRef.wasObjectDeleted()) effect->clearVizFeedback();
}

bool EffectChainVizUI::isReallyAffecting()
{
	return effect->isAffectingObject(object) && effect->enabled->boolValue() && effect->weight->floatValue() > 0;
}

String EffectChainVizUI::getVizLabel() const
{
	return baseItem->niceName + "\n(" + baseItem->parentContainer->parentContainer->niceName + ")";
}

void EffectChainVizUI::resized()
{
	BaseItemChainVizComponent::resized();
	Rectangle<int> r = getLocalBounds().reduced(2);

	if (paramUI != nullptr) paramUI->setBounds(r.removeFromBottom(16));

	r.removeFromLeft(30);
	weightUI->setBounds(r.removeFromTop(16));
}

void EffectChainVizUI::timerCallback()
{
	if (object == nullptr)
	{
		stopTimer();
		return;
	}

}