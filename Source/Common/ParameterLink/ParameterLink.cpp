/*
  ==============================================================================

	ParameterLink.cpp
	Created: 21 Dec 2020 11:12:38pm
	Author:  bkupe

  ==============================================================================
*/
#include "Common/CommonIncludes.h"
#include "Object/ObjectIncludes.h"

ParameterLink::ParameterLink(WeakReference<Parameter> p) :
	linkType(NONE),
	parameter(p),
	isLinkable(true),
	spatializer(nullptr),
	isLinkBeingDestroyed(false),
	//replacementHasMappingInputToken(false),
	paramLinkNotifier(5)
{

}

ParameterLink::~ParameterLink()
{
	if (ObjectManager::getInstanceWithoutCreating() != nullptr) ObjectManager::getInstance()->spatializer.removeBaseManagerListener(this);
	//if (list != nullptr && !listRef.wasObjectDeleted())
	//{
	//	list->removeListListener(this);
	//}
}

void ParameterLink::setLinkType(LinkType type)
{
	if (type == linkType) return;
	linkType = type;

	if (linkType != SPAT_X && linkType != SPAT_Z && linkType != SPAT_XZ)
	{
		spatializer = nullptr;
		spatRef = nullptr;
		if (ObjectManager::getInstanceWithoutCreating() != nullptr) ObjectManager::getInstance()->spatializer.removeBaseManagerListener(this);
	}

	parameter->setControllableFeedbackOnly(linkType != NONE);

	notifyLinkUpdated();
}

void ParameterLink::setSpatLink(LinkType type, SpatItem* spat)
{
	if (type == linkType && spat == spatializer) return;
	spatializer = spat;
	spatRef = spatializer;


	if (spatializer != nullptr)
	{
		ObjectManager::getInstance()->spatializer.addBaseManagerListener(this);
		spatGhostName = "";
	}

	setLinkType(type);
}

void ParameterLink::setLinkedCustomParam(Parameter* p)
{
	if (linkType == CUSTOM_PARAM && linkedCustomParam == p) return;
	linkedCustomParam = p;
	setLinkType(CUSTOM_PARAM);
}

var ParameterLink::getLinkedValue(Object* o, int id)
{
	switch (linkType)
	{
	case NONE: return parameter->getValue();
	case OBJECT_ID: return id;
	case CUSTOM_PARAM:
	{
		if (linkedCustomParam == nullptr || linkedCustomParam.wasObjectDeleted()) return parameter->getValue();
		return o->customParams->getParamValueFor(linkedCustomParam);
	}
	break;

	case OBJECT_POSX:
		return o->stagePosition->x;
	case OBJECT_POSY:
		return o->stagePosition->y;

	case OBJECT_POSZ:
		return o->stagePosition->z;

	case OBJECT_POSXZ:
	{
		var val;
		val.append(o->stagePosition->x);
		if (parameter->value.size() > 1) val.append(o->stagePosition->z);
		return val;
	}
	case OBJECT_POSXYZ:
	{
		var val;
		val.append(o->stagePosition->x);
		if (parameter->value.size() > 1) val.append(o->stagePosition->y);
		if (parameter->value.size() > 2) val.append(o->stagePosition->z);

		return val;
	}

	case SPAT_X:
	case SPAT_Z:
	case SPAT_XZ:
	{
		if (spatializer != nullptr && !spatRef.wasObjectDeleted())
		{
			Point<float> relPos = spatializer->getObjectPosition(o);
			if (linkType == SPAT_X) return relPos.x;
			else if (linkType == SPAT_Z) return relPos.y;
			else
			{
				var val;
				val.append(relPos.x);
				if (parameter->value.size() > 1) val.append(relPos.y);
				return val;
			}
		}
	}
	}

	return parameter->getValue();
}





WeakReference<Controllable> ParameterLink::getLinkedTarget(Object* o)
{
	if (!isLinkable)
	{
		if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->target;
		return nullptr;
	};

	if (linkType == CUSTOM_PARAM)
	{
		if (TargetParameter* p = dynamic_cast<TargetParameter*>(o->customParams->getActiveParamFor(linkedCustomParam))) return p->target;
	}

	if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->target;
	return nullptr;
}

WeakReference<ControllableContainer> ParameterLink::getLinkedTargetContainer(Object* o)
{
	if (!isLinkable)
	{
		if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->targetContainer;
		return nullptr;
	};

	if (linkType == CUSTOM_PARAM)
	{
		if (TargetParameter* p = dynamic_cast<TargetParameter*>(o->customParams->getActiveParamFor(linkedCustomParam))) return p->targetContainer;
	}

	if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->targetContainer;
	return nullptr;
}

//String ParameterLink::getReplacementString(int multiplexIndex)
//{
//	replacementHasMappingInputToken = false;
//	if (parameter->type != parameter->STRING) return parameter->stringValue();
//
//	std::string s = parameter->stringValue().toStdString();
//	std::regex source_regex("\\{(\\w+:\\w+|index0?)\\}");
//
//	auto source_begin = std::sregex_iterator(s.begin(), s.end(), source_regex);
//	auto source_end = std::sregex_iterator();
//
//	if (std::distance(source_begin, source_end) == 0) return parameter->stringValue();
//
//	int lastPos = 0;
//	String result = "";
//
//	for (std::sregex_iterator i = source_begin; i != source_end; ++i)
//	{
//		std::smatch m = *i;
//		String matchStr(m.str());
//
//		result += parameter->stringValue().substring(lastPos, (int)m.position());
//
//		if (matchStr == "{index}" && isMultiplexed())  result += String(multiplexIndex + 1);
//		else if (matchStr == "{index0}" && isMultiplexed())  result += String(multiplexIndex);
//		else
//		{
//			StringArray dotSplit;
//			dotSplit.addTokens(matchStr.substring(1, matchStr.length() - 1), ":", "");
//			if (dotSplit.size() == 2)
//			{
//				if (dotSplit[0] == "list")
//				{
//					if (isMultiplexed())
//					{
//						if (BaseMultiplexList* curList = multiplex->listManager.getItemWithName(dotSplit[1]))
//						{
//
//							if (Parameter* lp = dynamic_cast<Parameter*>(curList->list[multiplexIndex]))
//							{
//								result += lp->stringValue();
//							}
//							else
//							{
//								result += lp->shortName; // show shortName for triggers, might be useful
//							}
//						}
//					}
//				}
//				else if (dotSplit[0] == "input")
//				{
//					replacementHasMappingInputToken = true;
//					int valueIndex = dotSplit[1].getIntValue() - 1; //1-based to be compliant with UI naming
//					if (mappingValues.size() > 0 && valueIndex >= 0 && valueIndex < mappingValues[multiplexIndex].size()) result += mappingValues[multiplexIndex][valueIndex].toString();
//					else result += "[bad index : " + String(valueIndex) + "]";
//				}
//				else result += matchStr;
//			}
//			else result += matchStr;
//		}
//
//		lastPos = (int)m.position() + matchStr.length();
//	}
//
//	result += parameter->stringValue().substring(lastPos);
//
//	return result;
//}


void ParameterLink::itemAdded(SpatItem* item)
{
	if (linkType != SPAT_X && linkType != SPAT_Z && linkType != SPAT_XZ) return;
	if (spatializer != nullptr && !spatRef.wasObjectDeleted()) return;
	if (spatGhostName == item->shortName) setSpatLink(linkType, item);
}

void ParameterLink::itemsAdded(Array<SpatItem*> items)
{
	if (linkType != SPAT_X && linkType != SPAT_Z && linkType != SPAT_XZ) return;
	if (spatializer != nullptr && !spatRef.wasObjectDeleted()) return;

	for (auto& item : items)
	{
		if (spatGhostName == item->shortName)
		{
			setSpatLink(linkType, item);
			return;
		}
	}
}

void ParameterLink::itemRemoved(SpatItem* item)
{
	if (linkType != SPAT_X && linkType != SPAT_Z && linkType != SPAT_XZ) return;
	if (spatializer == item)
	{
		spatGhostName = spatializer->shortName;
		spatializer = nullptr;
		spatRef = nullptr;
	}
}

void ParameterLink::itemsRemoved(Array<SpatItem*> items)
{
	if (linkType != SPAT_X && linkType != SPAT_Z && linkType != SPAT_XZ) return;
	for (auto& item : items)
	{
		if (spatializer == item)
		{
			spatGhostName = spatializer->shortName;
			spatializer = nullptr;
			spatRef = nullptr;
			return;
		}
	}
}

void ParameterLink::notifyLinkUpdated()
{
	parameterLinkListeners.call(&ParameterLinkListener::linkUpdated, this);
	paramLinkNotifier.addMessage(new ParameterLinkEvent(ParameterLinkEvent::PREVIEW_UPDATED, this));
}


var ParameterLink::getJSONData(bool includeNonOverriden)
{
	var data(new DynamicObject());
	if (isLinkable)
	{
		data.getDynamicObject()->setProperty("linkType", linkType);
		if (linkType == CUSTOM_PARAM) data.getDynamicObject()->setProperty("linkedCustomParamName", linkedCustomParam->shortName);
		if (spatializer != nullptr && !spatRef.wasObjectDeleted()) data.getDynamicObject()->setProperty("spatializer", spatializer->shortName);
	}

	return data;
}

void ParameterLink::loadJSONData(var data)
{
	setLinkType((LinkType)(int)data.getProperty("linkType", NONE));

	if (!data.isObject() || !isLinkable)  return;
	if (linkType == CUSTOM_PARAM)
	{
		if (GenericControllableItem* gci = ObjectManager::getInstance()->customParams.getItemWithName(data.getProperty("linkedCustomParamName", "")))
		{
			linkedCustomParam = (Parameter*)gci->controllable;
		}
	}
	else if (linkType == SPAT_X || linkType == SPAT_Z || linkType == SPAT_XZ)
	{
		spatializer = ObjectManager::getInstance()->spatializer.getItemWithName(data.getProperty("spatializer", ""));
		spatRef = spatializer;

		if (spatializer == nullptr) spatGhostName = data.getProperty("spatializer", "");	
	}
}

ParamLinkContainer::ParamLinkContainer(const String& name) :
	ControllableContainer(name),
	paramsCanBeLinked(true),
	ghostData(new DynamicObject())
{
}

ParamLinkContainer::~ParamLinkContainer()
{
	paramLinkMap.clear();
	paramLinks.clear();
}

void ParamLinkContainer::onControllableAdded(Controllable* c)
{
	if (!paramsCanBeLinked) return;

	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		ParameterLink* pLink = new ParameterLink(p);

		pLink->addParameterLinkListener(this);

		paramLinks.add(pLink);
		paramLinkMap.set(p, pLink);
		linkParamMap.set(pLink, p);

		if (ghostData.hasProperty(pLink->parameter->shortName))
		{
			pLink->loadJSONData(ghostData.getProperty(pLink->parameter->shortName, var()));
		}
	}
}

void ParamLinkContainer::onControllableRemoved(Controllable* c)
{
	if (!paramsCanBeLinked) return;
	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		if (paramLinkMap.contains(p))
		{
			if (ParameterLink* pLink = paramLinkMap[p])
			{
				if (ghostData.isVoid()) ghostData = new DynamicObject();
				ghostData.getDynamicObject()->setProperty(pLink->parameter->shortName, pLink->getJSONData());

				pLink->removeParameterLinkListener(this);

				linkParamMap.remove(pLink);
				paramLinkMap.remove(p);
				paramLinks.removeObject(pLink);
			}
		}
	}
}

void ParamLinkContainer::parameterControlModeChanged(Parameter* p)
{
	paramLinkContainerListeners.call(&ParamLinkContainerListener::paramControlModeChanged, this, getLinkedParam(p));
}

ParameterLink* ParamLinkContainer::getLinkedParam(Parameter* p)
{
	if (!paramsCanBeLinked) return nullptr;

	jassert(paramLinkMap.contains(p));
	return paramLinkMap[p];
}

var ParamLinkContainer::getLinkedValue(Parameter* p, Object* o, int id, float time)
{
	if (p == nullptr) return var();
	if (!paramsCanBeLinked) return getParamValue(p, time);
	if (ParameterLink* pLink = getLinkedParam(p))
	{
		if (pLink->linkType != ParameterLink::NONE) return pLink->getLinkedValue(o, id);
	}
	return getParamValue(p, time);
}

var ParamLinkContainer::getParamValue(Parameter* p, float time)
{
	if (p->controlMode != Parameter::AUTOMATION) return p->getValue();

	if (ParameterAutomation* a = p->automation.get())
	{
		if (a->manualMode)
		{
			if (dynamic_cast<Automation*>(a->automationContainer) != nullptr)
			{
				float value = ((Automation*)a->automationContainer)->getValueAtPosition(fmodf(time, a->lengthParamRef->floatValue()));
				return value;
			}
			else if (dynamic_cast<GradientColorManager*>(a->automationContainer) != nullptr)
			{
				Colour value = ((GradientColorManager*)a->automationContainer)->getColorForPosition(fmodf(time, a->lengthParamRef->floatValue()));
				var result;
				result.append(value.getFloatRed());
				result.append(value.getFloatGreen());
				result.append(value.getFloatBlue());
				result.append(value.getFloatAlpha());
				return result;
			}
		}
	}

	return p->getValue();
}


void ParamLinkContainer::linkUpdated(ParameterLink* p)
{
	paramLinkContainerListeners.call(&ParamLinkContainerListener::linkUpdated, this, p);
}


var ParamLinkContainer::getJSONData(bool includeNonOverriden)
{
	var data = ControllableContainer::getJSONData(includeNonOverriden);

	var pLinkData(new DynamicObject());
	for (auto& pLink : paramLinks)
	{
		if (pLink->parameter.wasObjectDeleted() || pLink->parameter == nullptr) continue;
		if (pLink->linkType != pLink->NONE) pLinkData.getDynamicObject()->setProperty(pLink->parameter->shortName, pLink->getJSONData());
	}

	data.getDynamicObject()->setProperty("paramLinks", pLinkData);

	return data;
}

void ParamLinkContainer::loadJSONDataInternal(var data)
{
	ghostData = data.getProperty("paramLinks", var()).clone();
	for (auto& pLink : paramLinks)
	{
		if (pLink->parameter == nullptr || pLink->parameter.wasObjectDeleted()) continue;

		if (ghostData.isObject() && ghostData.hasProperty(pLink->parameter->shortName))
		{
			pLink->loadJSONData(ghostData.getProperty(pLink->parameter->shortName, var()));
			ghostData.getDynamicObject()->removeProperty(pLink->parameter->shortName);
		}
		else
		{
			pLink->setLinkType(ParameterLink::NONE);
		}
	}

}

InspectableEditor* ParamLinkContainer::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ParamLinkContainerEditor(this, isRoot);
}