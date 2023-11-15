/*
  ==============================================================================

	SceneHelpers.cpp
	Created: 9 Oct 2020 10:10:15am
	Author:  bkupe

  ==============================================================================
*/
#include "Common/CommonIncludes.h"

var SceneHelpers::getParamsSceneData(ControllableContainer* container, Array<Parameter*> excludeParams, bool recursive)
{
	var data(new DynamicObject());
	Array<WeakReference<Parameter>> params = container->getAllParameters(recursive);
	for (auto& p : params)
	{
		if (p->type == Parameter::ENUM || p->type == Parameter::TARGET) continue;
		if (p->isControllableFeedbackOnly) continue;
		if (BaseItem* bi = dynamic_cast<BaseItem*>(container))
		{
			if (p == bi->enabled || p == bi->listUISize || p == bi->viewUIPosition || p == bi->viewUISize || p == bi->miniMode || p == bi->isUILocked) continue;
		}

		if (excludeParams.contains(p.get())) continue;

		data.getDynamicObject()->setProperty(p->getControlAddress(container), p->value);
	}

	return data;
}

void SceneHelpers::lerpSceneParams(ControllableContainer* container, var startData, var endData, float weight, bool recursive)
{
	Array<WeakReference<Parameter>> params = container->getAllParameters(recursive);

	for (auto& p : params)
	{
		if (p.wasObjectDeleted() || p == nullptr) continue;
		if (p->type == Parameter::ENUM || p->type == Parameter::TARGET) continue;
		if (p->isControllableFeedbackOnly) continue;
		if (BaseItem* bi = dynamic_cast<BaseItem*>(container))
		{
			if (p == bi->enabled || p == bi->listUISize || p == bi->viewUIPosition || p == bi->viewUISize || p == bi->miniMode || p == bi->isUILocked) continue;
		}

		lerpSceneParam(container, p, startData, endData, weight);
	}
}

void SceneHelpers::lerpSceneParam(ControllableContainer* container, Parameter* p, var startData, var endData, float weight)
{
	var val = var();
	String addr = p->getControlAddress(container);
	var startValue = startData.getProperty(addr, p->defaultValue);
	var endValue = endData.getProperty(addr, p->defaultValue);

	if (p->value.isArray())
	{
		for (int i = 0; i < p->value.size(); i++)
		{
			val.append((float)startValue[i] + ((float)endValue[i] - (float)startValue[i]) * weight);
		}
	}
	else if (p->value.isDouble() || p->value.isInt())
	{
		val = (float)startValue + ((float)endValue - (float)startValue) * weight;
	}
	else if (p->value.isBool())
	{
		val = weight >= .5f ? startValue : endValue;
	}

	p->setValue(val);
}
