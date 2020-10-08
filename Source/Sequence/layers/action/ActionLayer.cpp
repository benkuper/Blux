/*
  ==============================================================================

    ActionLayer.cpp
    Created: 5 Oct 2020 4:02:52pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionLayer.h"
#include "../../BluxSequence.h"

#include "Scene/actions/SceneAction.h"
#include "Object/actions/ObjectAction.h"
#include "Effect/actions/EffectAction.h"
#include "Sequence/actions/SequenceAction.h"

ActionLayer::ActionLayer(Sequence* s, var params) :
    TriggerLayer(s, "Action", params)
{
    setManager(new ActionManager(this, s));
}

ActionLayer::~ActionLayer()
{
}


ActionManager::ActionManager(ActionLayer* layer, Sequence* sequence) :
    TimeTriggerManager(layer, sequence)
{
    managerFactory = ActionFactory::getInstance();
}

ActionManager::~ActionManager()
{
}


juce_ImplementSingleton(ActionFactory);

ActionFactory::ActionFactory()
{
    defs.add(Factory<TimeTrigger>::Definition::createDef("Scene", "Load Scene", &SceneAction::create)->addParam("type", SceneAction::LOAD_SCENE));
    defs.add(Factory<TimeTrigger>::Definition::createDef("Scene", "Load Next Scene", &SceneAction::create)->addParam("type", SceneAction::LOAD_NEXT_SCENE));
}
