/*
  ==============================================================================

	ComponentType.h
	Created: 9 Sep 2021 9:29:10pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

enum ComponentType { DIMMER, COLOR, SHUTTER, ORIENTATION, CUSTOM, TYPES_MAX };
const String componentTypeNames[TYPES_MAX]{ "Dimmer", "Color", "Shutter", "Orientation", "Custom" };

