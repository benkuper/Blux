/*
  ==============================================================================

	RawDataLayer.cpp
	Created: 12 Dec 2022 5:37:59pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"
#include "RawDataLayer.h"

RawDataLayer::RawDataLayer(Sequence* s, var params) :
	SequenceLayer(s, "Raw Data"),
	blockManager(this),
	timeAtRecord(0),
	numWrittenFrames(0),
	activeBlock(nullptr),
	needsToSendAllUniverses(true),
	dmxInterface(nullptr)
{
	saveAndLoadRecursiveData = true;
	addChildControllableContainer(&blockManager);

	targetInterface = addTargetParameter("targetInterface", "targetInterface", InterfaceManager::getInstance());
	targetInterface->targetType = TargetParameter::CONTAINER;
	targetInterface->maxDefaultSearchLevel = 0;
	targetInterface->typesFilter.add(DMXInterface::getTypeStringStatic());

	forceResetValues = addBoolParameter("Force Reset Values", "If checked, this will force a value to zero before resetting", false);

	arm = addBoolParameter("arm", "arm", false);
	autoDisarm = addBoolParameter("autoDisarm", "autoDisarm", true);

	recordToSave = addFileParameter("recordToSave", "recordToSave");
	recordToSave->setValue("records/sample.rawdata");
	recordToSave->saveMode = true;

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

	int dataSize = b.getSize(); // +4 + 4;//including curTime and numUniverses, not including packetSize part

	if (timeAtRecord == -1) timeAtRecord = sequence->currentTime->floatValue();
	float time = sequence->currentTime->floatValue() - timeAtRecord;

	output->writeInt(dataSize);
	output->writeFloat(time);
	output->writeInt(numUniverses);
	output->write(b.getData(), b.getSize());

	numWrittenFrames++;
}

void RawDataLayer::stopRecording()
{
	isRecording->setValue(false);

	if (output == nullptr) return;

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
		if (enabled->boolValue() && arm->boolValue()) startRecording();
	}
	else
	{
		if (isRecording->boolValue()) stopRecording();
		if (autoDisarm->boolValue()) arm->setValue(false);
	}

	needsToSendAllUniverses = true;
}

void RawDataLayer::sequenceCurrentTimeChanged(Sequence* s, float prevTime, bool)
{

	if (!enabled->boolValue()) return;

	if (isRecording->boolValue())
	{
		recordOneFrame();
		return;
	}

	if (dmxInterface == nullptr) return;

	float seqTime = sequence->currentTime->floatValue();
	if (LayerBlock* b = blockManager.getBlockAtTime(seqTime, false))
	{
		RawDataBlock* rb = (RawDataBlock*)b;

		{
			GenericScopedLock bLock(blockLock);
			if (rb != activeBlock)
			{
				activeBlock = rb;
				needsToSendAllUniverses = true;
			}
		}

		GenericScopedLock fLock(frameUniverses.getLock());
		frameUniverses.clear(); //only clear if block found, then when no block found, frameUniverses will keep memory of universes to send black to

		if (s->isSeeking || prevTime > s->currentTime->floatValue()) needsToSendAllUniverses = true;

		if (needsToSendAllUniverses)
		{
			//LOG("Read All Universes");
			frameUniverses.addArray(rb->readAllUniversesAtTime(seqTime));
			needsToSendAllUniverses = true; //switch to false to "optimize", but right now it doesn't work because it won't send data and overlay layers and objects may not blend good if some values have not been updated between frames
		}
		else
		{
			frameUniverses.addArray(rb->readFrameAtTime(seqTime));
		}

		if (frameUniverses.size() == 0)
		{
			LOGWARNING("No universe read");
		}
	}
	else
	{
		{
			GenericScopedLock bLock(blockLock);
			activeBlock = nullptr;
		}
	}
}

void RawDataLayer::dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName)
{
	if (!isRecording->boolValue()) return;
	if (!sequence->isPlaying->boolValue()) return;

	DMXUniverse* u = getUniverse(net, subnet, universe);

	jassert(u != nullptr);
	u->updateValues(values, true);
}

void RawDataLayer::processRawData()
{
	if (dmxInterface == nullptr) return;
	if (!enabled->boolValue()) return;
	if (isRecording->boolValue()) return;

	std::function<int(int, int)> blendFunc = nullptr;

	float fade = 0;
	RawDataBlock::BlendMode m = RawDataBlock::ALPHA;

	{
		GenericScopedLock bLock(blockLock);
		if (activeBlock != nullptr)
		{

			m = activeBlock->blendMode->getValueDataAsEnum<RawDataBlock::BlendMode>();
			fade = activeBlock->getFadeFactorAtTime(sequence->currentTime->floatValue());

			if (fade != 1 || m != RawDataBlock::ALPHA)
			{
				switch (m)
				{
				case RawDataBlock::ALPHA:
					blendFunc = [](int a, int b) { return b; };
					break;
				case RawDataBlock::ADD:
					blendFunc = [](int a, int b) { return jmin(a + b, 255); };
					break;
				case RawDataBlock::MULTIPLY:
					blendFunc = [](int a, int b) {return (int)(a * (b * 1.0f / 255.0f)); };
					break;
				case RawDataBlock::MAX:
					blendFunc = [](int a, int b) {return jmax(a, b); };
					break;
				case RawDataBlock::MIN:
					blendFunc = [](int a, int b) {return jmin(a, b); };
					break;
				}
			}
		}
	}

	{
		GenericScopedLock fLock(frameUniverses.getLock());
		if (frameUniverses.size() == 0)
		{
			LOGWARNING("No universes here !");
		}

		for (auto& u : frameUniverses)
		{
			DMXUniverse* interfaceU = dmxInterface->getUniverse(u->net, u->subnet, u->universe); //will force creation of the universe if doesn't exist

			{
				GenericScopedLock bLock(blockLock);
				if (activeBlock == nullptr) continue; //if not active block, only checking universe and creating one if necessary, will be filled with zeros
			}

			//ddGenericScopedLock uLock(u->valuesLock);

			if (fade == 1 && m == RawDataBlock::ALPHA)
			{
				interfaceU->updateValues(u->values);
			}
			else
			{
				//GenericScopedLock lock(interfaceU->valuesLock);

				//if (forceResetValues->boolValue())
				//	interfaceU->values.fill(0);

				for (int i = 0; i < DMX_NUM_CHANNELS; i++)
				{
					int targetVal = blendFunc(interfaceU->values[i], u->values[i]);
					if (fade != 1) targetVal = (int)jmap<float>(fade, 0, 1, interfaceU->values[i], targetVal);
					interfaceU->values.set(i, targetVal);
				}

				interfaceU->isDirty = true;

			}
		}
	}
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

SequenceLayerPanel* RawDataLayer::getPanel()
{
	return new RawDataLayerPanel(this);
}

SequenceLayerTimeline* RawDataLayer::getTimelineUI()
{
	return new RawDataLayerTimeline(this);
}

