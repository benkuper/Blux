/*
  ==============================================================================

	RawDataLayer.cpp
	Created: 12 Dec 2022 5:37:59pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataLayer::RawDataLayer(Sequence* s, var params) :
	SequenceLayer(s, "Raw Data"),
	blockManager(this),
	timeAtRecord(0),
	numWrittenFrames(0),
	dmxInterface(nullptr)
{
	saveAndLoadRecursiveData = true;
	addChildControllableContainer(&blockManager);

	targetInterface = addTargetParameter("targetInterface", "targetInterface", InterfaceManager::getInstance());
	targetInterface->targetType = TargetParameter::CONTAINER;
	targetInterface->maxDefaultSearchLevel = 0;
	targetInterface->typesFilter.add(DMXInterface::getTypeStringStatic());

	arm = addBoolParameter("arm", "arm", false);
	autoDisarm = addBoolParameter("autoDisarm", "autoDisarm", true);

	recordToSave = addFileParameter("recordToSave", "recordToSave");
	recordToSave->setValue("records/sample.rawdata");

	isRecording = addBoolParameter("Is Recording", "", false);
	isRecording->setControllableFeedbackOnly(true);

}

RawDataLayer::~RawDataLayer()
{
	setDMXInterface(nullptr);
}

void RawDataLayer::onContainerParameterChangedInternal(Parameter* p)
{
	SequenceLayer::onContainerParameterChangedInternal(p);
	if (p == targetInterface)
	{
		setDMXInterface(dynamic_cast<DMXInterface*>(targetInterface->targetContainer.get()));
	}
}

void RawDataLayer::setDMXInterface(DMXInterface* in)
{
	if (in == dmxInterface) return;

	if (dmxInterface != nullptr)
	{
		dmxInterface->removeDMXInterfaceListener(this);

	}

	dmxInterface = in;

	if (dmxInterface != nullptr)
	{
		dmxInterface->addDMXInterfaceListener(this);
	}
}

void RawDataLayer::startRecording()
{
	//reset universes
	universeIdMap.clear();
	universes.clear();

	recordingFile = recordToSave->getFile();
	if (recordingFile.existsAsFile()) recordingFile.deleteFile();

	output.reset(new FileOutputStream(recordingFile));

	//reserve space for metadata

	output->writeFloat(0); //totalTime
	output->writeInt(0); //total Num Universes
	output->writeInt(0); //num written frames

	numWrittenFrames = 0;

	timeAtRecord = -1;

	isRecording->setValue(true);
}


void RawDataLayer::recordOneFrame()
{
	if (output == nullptr) return;

	int numUniverses = 0;
	MemoryBlock b;
	MemoryOutputStream os(b, false);


	for (auto& u : universes)
	{
		if (!u->isDirty) continue;
		os.writeInt(u->getUniverseIndex());
		os.write(u->values.getRawDataPointer(), DMX_NUM_CHANNELS);
		u->isDirty = false;
		numUniverses++;
	}

	os.flush();

	if (numUniverses == 0) return;

	int packetSize = b.getSize() + 4 + 4;//including curTime and numUniverses, not including packetSize part

	if (timeAtRecord == -1) timeAtRecord = sequence->currentTime->floatValue();
	float time = sequence->currentTime->floatValue() - timeAtRecord;

	output->writeInt(packetSize);
	output->writeInt(time);
	output->writeInt(numUniverses);
	output->write(b.getData(), b.getSize());

	numWrittenFrames++;
}

void RawDataLayer::stopRecording()
{
	if (timeAtRecord == -1 || universes.size() == 0)
	{
		output->flush();
		output.reset();
		return;
	}

	output->setPosition(0);
	output->writeFloat(sequence->currentTime->floatValue() - timeAtRecord); //totalTime
	output->writeInt(universes.size()); //total Num Universes
	output->writeInt(numWrittenFrames); //num written frames


	output->flush();
	output.reset();

	isRecording->setValue(false);
	LOG("Record saved to " << recordingFile.getFullPathName());

	RawDataBlock* b = new RawDataBlock();
	b->time->setValue(timeAtRecord);
	b->fileParam->setValue(recordingFile.getFullPathName());
	blockManager.addItem(b);
}


void RawDataLayer::sequencePlayStateChanged(Sequence* s)
{
	if (s->isPlaying->boolValue())
	{
		if (arm->boolValue()) startRecording();
	}
	else
	{
		if (isRecording->boolValue()) stopRecording();
		if (autoDisarm->boolValue()) arm->setValue(false);
	}
}

void RawDataLayer::sequenceCurrentTimeChanged(Sequence*, float, bool)
{
	if (isRecording->boolValue()) recordOneFrame();
}

void RawDataLayer::dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName)
{
	if (!isRecording->boolValue()) return;
	if (!sequence->isPlaying->boolValue()) return;

	DMXUniverse* u = getUniverse(net, subnet, universe);

	jassert(u != nullptr);
	u->updateValues(values, true);
}

inline DMXUniverse* RawDataLayer::getUniverse(int net, int subnet, int universe, bool createIfNotExist)
{
	const int index = DMXUniverse::getUniverseIndex(net, subnet, universe);
	if (universeIdMap.contains(index)) return universeIdMap[index];

	if (!createIfNotExist) return nullptr;

	DMXUniverse* u = new DMXUniverse(net, subnet, universe);
	universes.add(u);
	universeIdMap.set(index, u);
	return universeIdMap[index];
}

SequenceLayerTimeline* RawDataLayer::getTimelineUI()
{
	return new RawDataLayerTimeline(this);
}

