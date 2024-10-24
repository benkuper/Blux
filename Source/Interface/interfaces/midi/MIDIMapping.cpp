/*
  ==============================================================================

    MIDIMapping.cpp
    Created: 12 Oct 2020 11:07:59am
    Author:  bkupe

  ==============================================================================
*/

MIDIMapping::MIDIMapping() :
    BaseItem("MIDI Mapping"),
    isValid(false),
    wasInRange(false)
{
    saveAndLoadRecursiveData = true;

    mode = addEnumParameter("Mode", "Set the mode of this mapping.");
    mode->addOption("Trigger", TRIGGER)->addOption("On / Off", ONOFF)->addOption("Toggle", TOGGLE)->addOption("Continuous", CONTINUOUS);

    midiType = addEnumParameter("Type", "Sets the type to check");
    midiType->addOption("Note", NOTE)->addOption("Control Change", CONTROLCHANGE);
    channel = addIntParameter("Channel", "The channel to use for this mapping.", 1, 1, 16);
    pitchOrNumber = addIntParameter("Pitch Or Number", "The pitch (for notes) or number (for controlChange) to use for this mapping.", 0, 0, 127);

    inputRange = addPoint2DParameter("Input Range", "The range to get from input");
    inputRange->setBounds(0, 0, 127, 127);
    inputRange->setPoint(0, 127);
    
    outputRange = addPoint2DParameter("Output Range", "The range to remap the value to.");
    outputRange->setPoint(0, 1);

    learnMode = addBoolParameter("Learn", "When active, this will automatically set the channel and pitch/number to the next incoming message", false);
    learnMode->isSavable = false;
    learnMode->hideInEditor = true;

    addChildControllableContainer(&actionManager);

    showInspectorOnSelect = false;
}

MIDIMapping::~MIDIMapping()
{
}

void MIDIMapping::handleNote(int channel, int pitch, int velocity)
{
    if (learnMode->boolValue())
    {
        midiType->setValueWithData(NOTE);
        pitchOrNumber->setValue(pitch);
        learnMode->setValue(false);
    }

    if (!enabled->boolValue()) return;

    if (midiType->getValueDataAsEnum<MidiType>() != NOTE) return;
    if (pitchOrNumber->intValue() != pitch) return;
    handleValue(velocity);
}

void MIDIMapping::handleCC(int channel, int number, int value)
{
    if (learnMode->boolValue())
    {
        midiType->setValueWithData(CONTROLCHANGE);
        pitchOrNumber->setValue(number);
        learnMode->setValue(false);
    }

    if (!enabled->boolValue()) return;
    if (midiType->getValueDataAsEnum<MidiType>() != CONTROLCHANGE) return;
    if (pitchOrNumber->intValue() != number) return;
    handleValue(value);
}

void MIDIMapping::handleValue(int value)
{
    if (!enabled->boolValue()) return;

    MappingMode m = mode->getValueDataAsEnum<MappingMode>();

    if (m != CONTINUOUS)
    {
        bool isInRange = value >= inputRange->x && value <= inputRange->y;
        if (wasInRange != isInRange) //state change
        {
            bool valid = isInRange;
            if (m == TRIGGER || m == ONOFF)
            {
                isValid = valid;
                if (m == TRIGGER && isValid) actionManager.triggerAll();
            }
            else if (m == TOGGLE)
            {
                if (valid) isValid = !isValid;
            }

            if (m == ONOFF || m == TOGGLE)
            {
                actionManager.setValueAll(isValid ? outputRange->y : outputRange->x);
            }

            wasInRange = isInRange;
        }
    }else
    {
        float minInput = jmin(inputRange->x, inputRange->y);
        float maxInput = jmax(inputRange->x, inputRange->y);
        float relVal = jmap<float>(jlimit<float>(minInput, maxInput, value), minInput, maxInput, 0, 1);
        if (inputRange->x > inputRange->y) relVal = 1 - relVal;
        float targetVal = jmap<float>(relVal, outputRange->x, outputRange->y);
        actionManager.setValueAll(targetVal);
    }
}

InspectableEditor* MIDIMapping::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
    return new MIDIMappingEditor(this, isRoot);
}
