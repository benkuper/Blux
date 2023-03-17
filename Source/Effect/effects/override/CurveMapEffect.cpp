/*
  ==============================================================================

	CurveMapFilter.cpp
	Created: 21 Jan 2022 2:57:12pm
	Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"

CurveMapEffect::CurveMapEffect(var params) :
	Effect(getTypeString(), params),
	automation("Curve")
{
	inputRange = effectParams.addPoint2DParameter("Input Range", "Range to take as input");
	outputRange = effectParams.addPoint2DParameter("Output Range", "Range to remap to");
	inputRange->setPoint(0, 1);
	outputRange->setPoint(0, 1);

	AutomationKey* k = automation.addKey(0, 0);
	k->easingType->setValueWithData(Easing::LINEAR);
	automation.addKey(1, 1);
	effectParams.addChildControllableContainer(&automation);
}

CurveMapEffect::~CurveMapEffect()
{
}

void CurveMapEffect::processComponentInternal(Object* o, ObjectComponent* c, const HashMap<Parameter*, var>& values, HashMap<Parameter*, var>& targetValues, int id, float time)
{
	var inR = GetLinkedValue(inputRange);
	var outR = GetLinkedValue(outputRange);

	HashMap<Parameter*, var>::Iterator it(values);
	while (it.next())
	{
		var val = it.getValue().clone();

		if (val.isArray())
		{
			for (int j = 0; j < values.size(); j++)
			{
				float normVal = 0;
				if (inR[0] != inR[1]) normVal = jmap<float>(val[j], inR[0], inR[1], 0, 1);
				float mapVal = automation.getValueAtPosition(normVal);
				val[j] = jmap<float>(mapVal, outR[0], outR[1]);
			}
		}
		else
		{
			float normVal = 0;
			if (inR[0] != inR[1]) normVal = jmap<float>(val, inR[0], inR[1], 0, 1);
			float mapVal = automation.getValueAtPosition(normVal);
			val = jmap<float>(mapVal, outR[0], outR[1]);
		}

		targetValues.set(it.getKey(), val);
	}
}