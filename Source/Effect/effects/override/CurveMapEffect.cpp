/*
  ==============================================================================

	CurveMapFilter.cpp
	Created: 21 Jan 2022 2:57:12pm
	Author:  bkupe

  ==============================================================================
*/

CurveMapEffect::CurveMapEffect(var params) :
	Effect(getTypeString(), params),
	automation("Curve")
{
	inputRange = effectParams.addPoint2DParameter("Input Range", "Range to take as input");
	outputRange = effectParams.addPoint2DParameter("Output Range", "Range to remap to");
	inputRange->setPoint(0, 1);
	outputRange->setPoint(0, 1);

	AutomationKey * k = automation.addKey(0, 0);
	k->easingType->setValueWithData(Easing::LINEAR);
	automation.addKey(1, 1);
	effectParams.addChildControllableContainer(&automation);

	filterManager->componentSelector.allowedComponents.removeAllInstancesOf(ComponentType::COLOR);
	filterManager->componentSelector.selectedComponents.set(ComponentType::INTENSITY, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::PANTILT, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::SERVO, true);
	filterManager->componentSelector.selectedComponents.set(ComponentType::STEPPER, true);
}

CurveMapEffect::~CurveMapEffect()
{
}

var CurveMapEffect::getProcessedComponentValuesInternal(Object* o, ObjectComponent* c, var values, int id, float time)
{
	var inR = GetLinkedValue(inputRange);
	var outR = GetLinkedValue(outputRange);


	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].isArray())
		{
			for (int j = 0; j < values.size(); j++)
			{
				float normVal = 0;
				if(inR[0] != inR[1]) normVal = jmap<float>(values[i][j], inR[0],inR[1], 0, 1);
				float mapVal = automation.getValueAtPosition(normVal);
				values[i][j] = jmap<float>(mapVal, outR[0], outR[1]);
			}
		}
		else
		{
			float normVal = 0;
			if (inR[0] != inR[1]) normVal = jmap<float>(values[i], inR[0], inR[1], 0, 1);
			float mapVal = automation.getValueAtPosition(normVal);
			values[i] = jmap<float>(mapVal, outR[0], outR[1]);
		}
	}

	return values;
}