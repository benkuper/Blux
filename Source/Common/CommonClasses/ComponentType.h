/*
  ==============================================================================

	ComponentType.h
	Created: 9 Sep 2021 9:29:10pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

enum ComponentType { INTENSITY, COLOR, GOBO, LASER, PAN, TILT, SERVO, STEPPER, STROBE, SCRIPT, CUSTOM, TYPES_MAX };
const String componentTypeNames[TYPES_MAX]{ "Intensity", "Color", "Gobo", "Laser", "Pan", "Tilt", "Servo", "Stepper", "Strobe", "Script", "Custom" };

