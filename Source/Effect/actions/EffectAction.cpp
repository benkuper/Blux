/*
  ==============================================================================

	EffectAction.cpp
	Created: 8 Oct 2020 3:31:34pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"
#include "Scene/SceneIncludes.h"

EffectAction::EffectAction(var params) :
	Action(params),
	weight(nullptr)
{
	actionType = (ActionType)(int)params.getProperty("actionType", ENABLE_EFFECT);

	effect = addTargetParameter("Scene", "The effect to target");
	effect->targetType = TargetParameter::CONTAINER;
	effect->customGetTargetContainerFunc = &EffectAction::showMenuAndGetEffect;

	if (actionType == SET_EFFECT_WEIGHT) weight = addFloatParameter("Weight", "The weight to set the effect to", 0, 0, 1);

}

EffectAction::~EffectAction()
{
}

void EffectAction::triggerInternal()
{
	Effect* e = (Effect*)effect->targetContainer.get();
	if (e == nullptr) return;

	switch (actionType)
	{
	case ENABLE_EFFECT: e->enabled->setValue(true); break;
	case DISABLE_EFFECT: e->enabled->setValue(false); break;
	case SET_EFFECT_WEIGHT: e->weight->setValue(weight->floatValue()); break;
	}
}

void EffectAction::setValueInternal(var value)
{
	Effect* e = (Effect*)effect->targetContainer.get();
	if (e == nullptr) return;

	switch (actionType)
	{
	case ENABLE_EFFECT:
	case DISABLE_EFFECT:
		e->enabled->setValue((float)value == 1);
		break;

	case SET_EFFECT_WEIGHT:
		if (!value.isArray())
		{
			weight->setValue(value);
			e->weight->setValue(weight->floatValue());
		}
		break;
	}
}

void EffectAction::showMenuAndGetEffect(ControllableContainer* fromCC, std::function<void(ControllableContainer*)> returnFunc)
{
	Array<Effect*> effects;
	PopupMenu m;

	PopupMenu globalMenu;
	for (auto& i : GlobalEffectManager::getInstance()->items)
	{
		PopupMenu effectGroupMenu;
		for (auto& e : i->effectManager.items)
		{
			effects.add(e);
			effectGroupMenu.addItem(effects.size(), e->niceName);
		}
		globalMenu.addSubMenu(i->niceName, effectGroupMenu);
	}

	m.addSubMenu("Global Effects", globalMenu);


	PopupMenu sceneMenu;
	for (auto& i : SceneManager::getInstance()->items)
	{
		PopupMenu sceneEffectsMenu;
		for (auto& e : i->effectManager->items)
		{
			effects.add(e);
			sceneEffectsMenu.addItem(effects.size(), e->niceName);
		}
		sceneMenu.addSubMenu(i->niceName, sceneEffectsMenu);
	}

	m.addSubMenu("Scenes", sceneMenu);


	PopupMenu groupsMenu;
	for (auto& i : GroupManager::getInstance()->items)
	{
		PopupMenu groupEffectsMenu;
		for (auto& e : i->effectManager->items)
		{
			effects.add(e);
			groupEffectsMenu.addItem(effects.size(), e->niceName);
		}
		groupsMenu.addSubMenu(i->niceName, groupEffectsMenu);
	}

	m.addSubMenu("Groups", groupsMenu);


	PopupMenu objectsMenu;
	for (auto& i : ObjectManager::getInstance()->items)
	{
		PopupMenu objectEffectsMenu;
		for (auto& e : i->effectManager->items)
		{
			effects.add(e);
			objectEffectsMenu.addItem(effects.size(), e->niceName);
		}
		objectsMenu.addSubMenu(i->niceName, objectEffectsMenu);
	}

	m.addSubMenu("Objects", objectsMenu);


	m.showMenuAsync(PopupMenu::Options(), [effects, returnFunc](int result)
		{
			if (result == 0) return;
			returnFunc(effects[result - 1]);
		}
	);
}


//Effect Manager

EffectManagerAction::EffectManagerAction(var params) :
	Action(params),
	weight(nullptr)
{
	actionType = (ActionType)(int)params.getProperty("actionType", SET_GROUP_EFFECT_WEIGHT);

	target = addTargetParameter("Scene", "The Effect Manager to target");
	target->targetType = TargetParameter::CONTAINER;
	target->maxDefaultSearchLevel = 0;
	switch (actionType)
	{
	case SET_GROUP_EFFECT_WEIGHT:
		target->setRootContainer(GroupManager::getInstance());
		break;

	case SET_GLOBAL_EFFECT_WEIGHT:
		target->setRootContainer(GlobalEffectManager::getInstance());
		break;
	}

	weight = addFloatParameter("Weight", "The weight to set the effect to", 0, 0, 1);

}

EffectManagerAction::~EffectManagerAction()
{
}

void EffectManagerAction::triggerInternal()
{
	if (effectManager == nullptr || effectManager->globalWeight == nullptr) return;
	effectManager->globalWeight->setValue(weight->floatValue());
}

void EffectManagerAction::setValueInternal(var value)
{
	if (effectManager == nullptr || effectManager->globalWeight == nullptr) return;

	weight->setValue(value);
	effectManager->globalWeight->setValue(weight->floatValue());
}

void EffectManagerAction::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == target)
	{
		if (ControllableContainer* cc = target->targetContainer.get())
		{
			if (Group* g = dynamic_cast<Group*>(cc))
				effectManager = g->effectManager.get();
			else if (EffectGroup* eg = dynamic_cast<EffectGroup*>(cc))
				effectManager = &eg->effectManager;
		}
		else
		{
			effectManager = nullptr;
		}
	}
}