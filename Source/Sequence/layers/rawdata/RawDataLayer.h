/*
  ==============================================================================

	RawDataLayer.h
	Created: 12 Dec 2022 5:37:59pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class RawDataLayer :
	public SequenceLayer,
	public DMXInterface::DMXInterfaceListener
{
public:
	RawDataLayer(Sequence* s, var params = var());
	~RawDataLayer();

	TargetParameter* targetInterface;
	BoolParameter* arm;
	BoolParameter* autoDisarm;
	FileParameter* recordToSave;

	BoolParameter* isRecording;

	enum FrameSendMode { ALL, ACTIVE, ACTIVE_AND_SEEK };
	EnumParameter* frameSendMode;
	BoolParameter* forceResetValues;

	File recordingFile;
	std::unique_ptr<FileOutputStream> output;

	RawDataBlockManager blockManager;
	RawDataBlock* activeBlock;

	OwnedArray<DMXUniverse> universes;
	HashMap<int, DMXUniverse*> universeIdMap; //internally used

	bool needsToSendAllUniverses;
	float timeAtRecord;
	int numWrittenFrames;
	DMXInterface* dmxInterface;

	void onContainerParameterChangedInternal(Parameter* p) override;

	void setDMXInterface(DMXInterface* in);

	void startRecording();
	void recordOneFrame();
	void stopRecording();

	virtual void sequencePlayStateChanged(Sequence*) override;
	virtual void sequenceCurrentTimeChanged(Sequence*, float prevTime, bool /*evaluateSkippedData*/) override;

	void dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName = "") override;

	DMXUniverse* getUniverse(int net, int subnet, int universe, bool createIfNotExist = true);

	SequenceLayerPanel* getPanel() override;
	SequenceLayerTimeline* getTimelineUI() override;

	DECLARE_TYPE("Raw Data");
	static RawDataLayer* create(Sequence* s, var params) { return new RawDataLayer(s, params); }

};