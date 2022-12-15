/*
  ==============================================================================

	RawDataBlock.h
	Created: 12 Dec 2022 5:38:15pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class UniverseRecord
{
public:
	UniverseRecord(int net, int subnet, int universe) :
		net(net), subnet(subnet), universe(universe)
	{}

	~UniverseRecord() {};

	int net;
	int subnet;
	int universe;

	double startTime;
	double endTime;

	struct ValueRecord
	{
		double time;
		uint8 values[DMX_NUM_CHANNELS];
	};

	Array<ValueRecord> values;
};

class RawDataBlock :
	public LayerBlock
{
public:
	RawDataBlock();
	~RawDataBlock();

	FileParameter* fileParam;
	File file;
	OwnedArray<UniverseRecord> rencords;

	std::unique_ptr<FileInputStream> fs;

	struct FrameData
	{
		float time = -1;
		int numUniverses = 0;
		int filePos = 0;
	};

	Array<FrameData> frames;

	int lastReadFrameIndex;

	void onContainerParameterChangedInternal(Parameter* p) override;

	void readInfos();
	Array<DMXUniverse *> readAtTime(float time);

	FrameData getFrameDataAtTime(float time);

	DECLARE_ASYNC_EVENT(RawDataBlock, RawDataBlock, rawData, ENUM_LIST(LOADED));
	DECLARE_TYPE("Raw Data Block");
};