/*
  ==============================================================================

	EffectAction.cpp
	Created: 8 Oct 2020 3:31:34pm
	Author:  bkupe

  ==============================================================================
*/

#include "EffectAction.h"
#include "Effect/GlobalEffectManager.h"
#include "Scene/SceneManager.h"
#include "Object/ObjectManager.h"
#include "Group/GroupManager.h"

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

ControllableContainer* EffectAction::showMenuAndGetEffect()
{
	Array<Effect *> effects;
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
		for (auto& e : i->effectManager.items)
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
		for (auto& e : i->effectManager.items)
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
		for (auto& e : i->effectManager.items)
		{
			effects.add(e);
			objectEffectsMenu.addItem(effects.size(), e->niceName);
		}
		objectsMenu.addSubMenu(i->niceName, objectEffectsMenu);
	}

	m.addSubMenu("Objects", objectsMenu);


	int result = m.show();
	return result == 0 ? nullptr : effects[result - 1];
}
