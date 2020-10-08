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
#include "Engine/GenericAction.h"

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
    defs.add(Factory<TimeTrigger>::Definition::createDef("Scene", "Load Scene", &SceneAction::create)->addParam("actionType", SceneAction::LOAD_SCENE));
    defs.add(Factory<TimeTrigger>::Definition::createDef("Scene", "Load Next Scene", &SceneAction::create)->addParam("actionType", SceneAction::LOAD_NEXT_SCENE));

    defs.add(Factory<TimeTrigger>::Definition::createDef("Effect", "Enable Effect", &EffectAction::create)->addParam("actionType", EffectAction::ENABLE_EFFECT));
    defs.add(Factory<TimeTrigger>::Definition::createDef("Effect", "Disable Effect", &EffectAction::create)->addParam("actionType", EffectAction::DISABLE_EFFECT));
    defs.add(Factory<TimeTrigger>::Definition::createDef("Effect", "Set Effect Weight", &EffectAction::create)->addParam("actionType", EffectAction::SET_EFFECT_WEIGHT));

    defs.add(Factory<TimeTrigger>::Definition::createDef("Sequence", "Play Sequence", &SequenceAction::create)->addParam("actionType", SequenceAction::PLAY_SEQUENCE));
    defs.add(Factory<TimeTrigger>::Definition::createDef("Sequence", "Stop Sequence", &SequenceAction::create)->addParam("actionType", SequenceAction::STOP_SEQUENCE));

    defs.add(Factory<TimeTrigger>::Definition::createDef("Generic", "Set Parameter Value", &GenericAction::create)->addParam("actionType", GenericAction::SET_VALUE));
    defs.add(Factory<TimeTrigger>::Definition::createDef("Generic", "Trigger a control", &GenericAction::create)->addParam("actionType", GenericAction::TRIGGER));

}
