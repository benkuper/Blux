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

	OwnedArray<UniverseRecord> records;

	DECLARE_ASYNC_EVENT(RawDataBlock, RawDataBlock, rawData, ENUM_LIST(LOADED))
	DECLARE_TYPE("Raw Data Block");
};